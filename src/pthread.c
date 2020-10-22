/*-
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 *  Copyright (c) 2020, MayaData Inc. All rights reserved.
 *  Copyright (c) 2020, DataCore Software Corporation. All rights reserved.
 * 
 *  POSIX details are based on the Open Group Base Specification Issue 7,
 *  2018 edition at https://pubs.opengroup.org/onlinepubs/9699919799/
 * 
 *  Details about Linux extensions are based on the Linux man-pages project
 *  at https://www.kernel.org/doc/man-pages/
 * 
 *  The static initialization of critical sections and condition variables
 *  depends upon undocumented Windows internals and is based upon ideas
 *  found at https://locklessinc.com/articles/pthreads_on_windows/
 */

#include <wpdk/internal.h>
#include <sys/time.h>
#include <process.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>


/*
 *  Verify that the public structure definitions match the
 *  underlying implementation. This avoids the need to include
 *  windows.h in pthread.h.
 */
#define CHECK_SIZE(x,y) \
	static_assert(sizeof(x)==sizeof(y), "Incorrect size for " #x)

CHECK_SIZE(pthread_mutex_t, CRITICAL_SECTION);
CHECK_SIZE(pthread_spinlock_t, CRITICAL_SECTION);
CHECK_SIZE(pthread_barrier_t, SYNCHRONIZATION_BARRIER);
CHECK_SIZE(pthread_cond_t, CONDITION_VARIABLE);


/*
 *  Default initialisation value to ensure that all mutexes are
 *  initialised identically. Includes a spin count.
 */ 
static const CRITICAL_SECTION mutex_init = PTHREAD_MUTEX_INITIALIZER;

static bool spdk_mutex_workarounds = true;
static SRWLOCK mutex_init_lock = SRWLOCK_INIT;

struct thread {
	DWORD id;
	HANDLE h;
	void *(*start)(void *);
	void *arg;
	void *result;
	int index;
};

static struct thread **wpdk_threads;
static SRWLOCK wpdk_threads_lock = SRWLOCK_INIT;

static int hwmthreads = 0;
static const int maxthreads = 8192;


static struct thread *
wpdk_allocate_thread()
{
	struct thread *pthread = NULL;
	int i;

	AcquireSRWLockExclusive(&wpdk_threads_lock);

	if (!wpdk_threads)
		wpdk_threads = calloc(maxthreads, sizeof(struct thread *));

	if (wpdk_threads)
		pthread = calloc(1, sizeof(struct thread));

	if (!pthread) {
		ReleaseSRWLockExclusive(&wpdk_threads_lock);
		wpdk_posix_error(ENOMEM);
		return NULL;
	}

	for (i = 0; i < maxthreads; i++)
		if (wpdk_threads[i] == NULL) {
			wpdk_threads[i] = pthread;
			pthread->index = i;
			if (i >= hwmthreads)
				hwmthreads = i + 1;
			break;
		}

	ReleaseSRWLockExclusive(&wpdk_threads_lock);

	if (i >= maxthreads) {
		wpdk_posix_error(EMFILE);
		free(pthread);
		return NULL;
	}

	return pthread;
}


static void
wpdk_set_thread_info(struct thread *pthread, DWORD id, HANDLE h, bool detached)
{
	AcquireSRWLockExclusive(&wpdk_threads_lock);

	pthread->id = id;
	pthread->h = (detached) ? 0 : h;

	if (pthread->h == 0) {
		wpdk_threads[pthread->index] = NULL;
		pthread->index = -1;
	}

	ReleaseSRWLockExclusive(&wpdk_threads_lock);

	if (h == 0) free(pthread);
}


static bool
wpdk_get_thread_info(DWORD id, HANDLE *phandle, int *pindex)
{
	bool rc = false;
	int i;

	AcquireSRWLockExclusive(&wpdk_threads_lock);

	for (i = 0; wpdk_threads && i < hwmthreads; i++)
		if (wpdk_threads[i] && wpdk_threads[i]->id == id) {
			*phandle = wpdk_threads[i]->h;
			*pindex = i;
			rc = true;
			break;
		}

	ReleaseSRWLockExclusive(&wpdk_threads_lock);
	return rc;
}


static void
wpdk_thread_starting(struct thread *pthread, struct thread *info)
{
	AcquireSRWLockExclusive(&wpdk_threads_lock);

	*info = *pthread;
	pthread->start = NULL;

	ReleaseSRWLockExclusive(&wpdk_threads_lock);

	if (info->index == -1)
		free(pthread);
}


static void
wpdk_thread_done(int index, void *result)
{
	DWORD id = GetCurrentThreadId();
	int i = index;

	AcquireSRWLockExclusive(&wpdk_threads_lock);

	if (i == -1 && wpdk_threads)
		for (i = 0; i < hwmthreads; i++)
			if (wpdk_threads[i] && wpdk_threads[i]->id == id) break;

	if (0 <= i && i < maxthreads && wpdk_threads)
		if (wpdk_threads[i] && wpdk_threads[i]->id == id)
			wpdk_threads[i]->result = result;

	ReleaseSRWLockExclusive(&wpdk_threads_lock);
}


static bool
wpdk_detach_thread(DWORD id, int index, void **presult)
{
	struct thread *pthread = NULL;
	void *(*start)(void *) = NULL;
	int i = index;
	HANDLE h = 0;

	AcquireSRWLockExclusive(&wpdk_threads_lock);

	if (i == -1 && wpdk_threads)
		for (i = 0; i < hwmthreads; i++)
			if (wpdk_threads[i] && wpdk_threads[i]->id == id) break;

	if (0 <= i && i < maxthreads && wpdk_threads)
		if (wpdk_threads[i] && wpdk_threads[i]->id == id) {
			pthread = wpdk_threads[i];
			pthread->index = -1;
			wpdk_threads[i] = NULL;
			start = pthread->start;
			h = pthread->h;
			if (presult) *presult = pthread->result;
		}

	ReleaseSRWLockExclusive(&wpdk_threads_lock);

	if (pthread) {
		if (h != 0) CloseHandle(h);
		if (start == NULL) free(pthread);
	}

	return (pthread != NULL);
}


int
wpdk_pthread_mutexattr_init(pthread_mutexattr_t *attr)
{
	if (!attr) return EINVAL;

	attr->type = PTHREAD_MUTEX_DEFAULT;
	attr->robust = PTHREAD_MUTEX_STALLED;
	attr->pshared = PTHREAD_PROCESS_PRIVATE;
	attr->protocol = PTHREAD_PRIO_NONE;
	return 0;
}


int
wpdk_pthread_mutexattr_destroy(pthread_mutexattr_t *attr)
{
	if (!attr) return EINVAL;
	return 0;
}


int
wpdk_pthread_mutexattr_gettype(const pthread_mutexattr_t *attr, int *type)
{
	if (!attr || !type)
		return EINVAL;

	*type = attr->type;
	return 0;
}


int
wpdk_pthread_mutexattr_settype(pthread_mutexattr_t *attr, int type)
{
	if (!attr) return EINVAL;

	switch (type) {
		case PTHREAD_MUTEX_RECURSIVE:
		case PTHREAD_MUTEX_NORMAL:
		case PTHREAD_MUTEX_ERRORCHECK:
			attr->type = type;
			return 0;
	}

	return EINVAL;
}


int
wpdk_pthread_mutexattr_getrobust(const pthread_mutexattr_t *attr, int *robust)
{
	if (!attr || !robust)
		return EINVAL;

	*robust = attr->robust;
	return 0;
}


int
wpdk_pthread_mutexattr_setrobust(pthread_mutexattr_t *attr, int robust)
{
	if (!attr) return EINVAL;

	switch (robust) {
		case PTHREAD_MUTEX_ROBUST:
		case PTHREAD_MUTEX_STALLED:
			attr->robust = robust;
			return 0;
	}

	return EINVAL;
}


int
wpdk_pthread_mutexattr_getpshared(const pthread_mutexattr_t *attr, int *pshared)
{
	if (!attr || !pshared)
		return EINVAL;

	*pshared = attr->pshared;
	return 0;
}


int
wpdk_pthread_mutexattr_setpshared(pthread_mutexattr_t *attr, int pshared)
{
	if (!attr) return EINVAL;

	switch (pshared) {
		case PTHREAD_PROCESS_PRIVATE:
		case PTHREAD_PROCESS_SHARED:
			attr->pshared = pshared;
			return 0;
	}

	return EINVAL;
}


int
wpdk_pthread_mutexattr_getprotocol(const pthread_mutexattr_t *attr, int *protocol)
{
	if (!attr || !protocol)
		return EINVAL;

	*protocol = attr->protocol;
	return 0;
}


int
wpdk_pthread_mutexattr_setprotocol(pthread_mutexattr_t *attr, int protocol)
{
	if (!attr) return EINVAL;

	switch (protocol) {
		case PTHREAD_PRIO_INHERIT:
		case PTHREAD_PRIO_PROTECT:
			WPDK_UNIMPLEMENTED();
			return ENOSYS;

		case PTHREAD_PRIO_NONE:
			attr->protocol = protocol;
			return 0;
	}

	return EINVAL;
}


int
wpdk_pthread_mutexattr_getprioceiling(const pthread_mutexattr_t *attr, int *prioceiling)
{
	if (!attr || !prioceiling)
		return EINVAL;

	WPDK_UNIMPLEMENTED();
	return ENOSYS;
}


int
wpdk_pthread_mutexattr_setprioceiling(pthread_mutexattr_t *attr, int prioceiling)
{
	UNREFERENCED_PARAMETER(prioceiling);

	if (!attr) return EINVAL;

	WPDK_UNIMPLEMENTED();
	return ENOSYS;
}


int
wpdk_pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *mutexattr)
{
	UNREFERENCED_PARAMETER(mutexattr);
	
	if (!mutex) return EINVAL;

	memcpy(mutex, &mutex_init, sizeof(*mutex));
	return 0;
}


int
wpdk_pthread_mutex_destroy(pthread_mutex_t *mutex)
{
	if (!mutex) return EINVAL;

	DeleteCriticalSection((CRITICAL_SECTION *)mutex);

	/*
	 *  Reinitialise the mutex to handle the SPDK Unit Tests
	 *  which can reuse it without calling init().
	 */
	if (spdk_mutex_workarounds)
		memcpy(mutex, &mutex_init, sizeof(*mutex));

	return 0;
}


int
wpdk_pthread_mutex_lock(pthread_mutex_t *mutex)
{
	CRITICAL_SECTION *lock = (CRITICAL_SECTION *)mutex;

	if (!mutex) return EINVAL;

	/*
	 *  The SPDK Unit Tests can use the mutex without initialising it.
	 *  If the DebugInfo field is zero then assume this has happened and
	 *  initialise the lock in a threadsafe manner.
	 */
	if (spdk_mutex_workarounds && !lock->DebugInfo) {
		AcquireSRWLockExclusive(&mutex_init_lock);
		memset(mutex, 0, sizeof(*mutex));
		lock->LockCount = -1;
		lock->SpinCount = 4000;
		InterlockedExchangePointer((void **)&lock->DebugInfo, (void *)-1);
		ReleaseSRWLockExclusive(&mutex_init_lock);
	}

	EnterCriticalSection(lock);
	return 0;
}


int
wpdk_pthread_mutex_trylock(pthread_mutex_t *mutex)
{
	if (!mutex) return EINVAL;

	if (TryEnterCriticalSection((CRITICAL_SECTION *)mutex) == 0)
		return EBUSY;

	return 0;
}


int
wpdk_pthread_mutex_unlock(pthread_mutex_t *mutex)
{
	if (!mutex) return EINVAL;

	LeaveCriticalSection((CRITICAL_SECTION *)mutex);
	return 0;
}


int
wpdk_pthread_mutex_consistent(pthread_mutex_t *mutex)
{
	UNREFERENCED_PARAMETER(mutex);

	WPDK_UNIMPLEMENTED();
	return EINVAL;
}


int
wpdk_pthread_spin_init(pthread_spinlock_t *lock, int pshared)
{
	if (!lock) return EINVAL;

	switch (pshared) {
		case PTHREAD_PROCESS_PRIVATE:
		case PTHREAD_PROCESS_SHARED:
			break;

		default:
			return EINVAL;
	}

	InitializeCriticalSectionAndSpinCount((CRITICAL_SECTION *)lock, 32000);
	return 0;
}


int
wpdk_pthread_spin_destroy(pthread_spinlock_t *lock)
{
	if (!lock) return EINVAL;

	DeleteCriticalSection((CRITICAL_SECTION *)lock);

	/*
	 *  Reinitialise the lock to handle the SPDK Unit Tests
	 *  which can reuse it without calling init().
	 */
	if (spdk_mutex_workarounds)
		InitializeCriticalSectionAndSpinCount((CRITICAL_SECTION *)lock, 32000);

	return 0;
}


int
wpdk_pthread_spin_lock(pthread_spinlock_t *lock)
{
	if (!lock) return EINVAL;

	EnterCriticalSection((CRITICAL_SECTION *)lock);
	return 0;
}


int
wpdk_pthread_spin_trylock(pthread_spinlock_t *lock)
{
	if (!lock) return EINVAL;

	if (TryEnterCriticalSection((CRITICAL_SECTION *)lock) == 0)
		return EBUSY;

	return 0;
}


int
wpdk_pthread_spin_unlock(pthread_spinlock_t *lock)
{
	if (!lock) return EINVAL;

	LeaveCriticalSection((CRITICAL_SECTION *)lock);
	return 0;
}


int
wpdk_pthread_barrierattr_init(pthread_barrierattr_t *attr)
{
	if (!attr) return EINVAL;

	attr->pshared = PTHREAD_PROCESS_PRIVATE;
	return 0;
}


int
wpdk_pthread_barrierattr_destroy(pthread_barrierattr_t *attr)
{
	if (!attr) return EINVAL;
	return 0;
}


int
wpdk_pthread_barrierattr_getpshared(const pthread_barrierattr_t *attr, int *pshared)
{
	if (!attr || !pshared)
		return EINVAL;

	*pshared = attr->pshared;
	return 0;
}


int
wpdk_pthread_barrierattr_setpshared(pthread_barrierattr_t *attr, int pshared)
{
	if (!attr) return EINVAL;

	switch (pshared) {
		case PTHREAD_PROCESS_PRIVATE:
		case PTHREAD_PROCESS_SHARED:
			attr->pshared = pshared;
			return 0;
	}

	return EINVAL;
}


int
wpdk_pthread_barrier_init(pthread_barrier_t *barrier,
		const pthread_barrierattr_t *attr, unsigned count)
{
	UNREFERENCED_PARAMETER(attr);

	if (!barrier || count == 0)
		return EINVAL;

	InitializeSynchronizationBarrier(
		(SYNCHRONIZATION_BARRIER *)barrier, count, -1);
	return 0;
}


int
wpdk_pthread_barrier_destroy(pthread_barrier_t *barrier)
{
	if (!barrier) return EINVAL;

	DeleteSynchronizationBarrier((SYNCHRONIZATION_BARRIER *)barrier);
	return 0;
}


int
wpdk_pthread_barrier_wait(pthread_barrier_t *barrier)
{
	if (!barrier) return EINVAL;

	return (EnterSynchronizationBarrier(
			(SYNCHRONIZATION_BARRIER *)barrier,
			SYNCHRONIZATION_BARRIER_FLAGS_BLOCK_ONLY) == TRUE) ?
			PTHREAD_BARRIER_SERIAL_THREAD : 0;
}


int
wpdk_pthread_condattr_init(pthread_condattr_t *attr)
{
	if (!attr) return EINVAL;

	attr->pshared = PTHREAD_PROCESS_PRIVATE;
	return 0;
}


int
wpdk_pthread_condattr_destroy(pthread_condattr_t *attr)
{
	if (!attr) return EINVAL;
	return 0;
}


int
wpdk_pthread_condattr_getpshared(const pthread_condattr_t *attr, int *pshared)
{
	if (!attr || !pshared)
		return EINVAL;

	*pshared = attr->pshared;
	return 0;
}


int
wpdk_pthread_condattr_setpshared(pthread_condattr_t *attr, int pshared)
{
	if (!attr) return EINVAL;

	switch (pshared) {
		case PTHREAD_PROCESS_PRIVATE:
		case PTHREAD_PROCESS_SHARED:
			attr->pshared = pshared;
			return 0;
	}

	return EINVAL;
}


int
wpdk_pthread_cond_init(pthread_cond_t *cond, const pthread_condattr_t *attr)
{
	UNREFERENCED_PARAMETER(attr);

	if (!cond) return EINVAL;

	InitializeConditionVariable((CONDITION_VARIABLE *)cond);
	return 0;
}


int
wpdk_pthread_cond_destroy(pthread_cond_t *cond)
{
	if (!cond) return EINVAL;
	return 0;
}


int
wpdk_pthread_cond_signal(pthread_cond_t *cond)
{
	if (!cond) return EINVAL;

	WakeConditionVariable((CONDITION_VARIABLE *)cond);
	return 0;
}


int
wpdk_pthread_cond_broadcast(pthread_cond_t *cond)
{
	if (!cond) return EINVAL;

	WakeAllConditionVariable((CONDITION_VARIABLE *)cond);
	return 0;
}


static int
wpdk_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex, DWORD msecs)
{
	if (!cond || !mutex)
		return EINVAL;

	if (SleepConditionVariableCS((CONDITION_VARIABLE *)cond,
			(CRITICAL_SECTION *)mutex, msecs) != 0)
		return 0;

	if (GetLastError() == ERROR_TIMEOUT)
		return ETIMEDOUT;

	return 0;
}


int
wpdk_pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex)
{
	return wpdk_cond_wait(cond, mutex, INFINITE);
}


int
wpdk_pthread_cond_timedwait(pthread_cond_t *cond, pthread_mutex_t *mutex, const struct timespec *abstime)
{
	DWORD msecs;

	if (wpdk_abstime_to_msecs(abstime, &msecs) == -1)
		return errno;

	return wpdk_cond_wait(cond, mutex, msecs);
}


int
wpdk_pthread_attr_init(pthread_attr_t *attr)
{
	if (!attr) return EINVAL;

	attr->detachstate = PTHREAD_CREATE_JOINABLE;
	attr->stacksize = 0;
	return 0;
}


int
wpdk_pthread_attr_destroy(pthread_attr_t *attr)
{
	if (!attr) return EINVAL;
	return 0;
}


int
wpdk_pthread_attr_getdetachstate(const pthread_attr_t *attr, int *detachstate)
{
	if (!attr || !detachstate)
		return EINVAL;

	*detachstate = attr->detachstate;
	return 0;
}


int
wpdk_pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate)
{
	if (!attr) return EINVAL;

	switch (detachstate) {
		case PTHREAD_CREATE_JOINABLE:
		case PTHREAD_CREATE_DETACHED:
			attr->detachstate = detachstate;
			return 0;
	}

	return EINVAL;
}


int
wpdk_pthread_attr_getstacksize(const pthread_attr_t *attr, size_t *stacksize)
{
	if (!attr || !stacksize)
		return EINVAL;

	*stacksize = attr->stacksize;
	return 0;
}


int
wpdk_pthread_attr_setstacksize(pthread_attr_t *attr, size_t stacksize)
{
	if (!attr) return EINVAL;

	if (stacksize > UINT_MAX)
		return EINVAL;

	attr->stacksize = (unsigned int)stacksize;
	return 0;
}


pthread_t
wpdk_pthread_self()
{
	return GetCurrentThreadId();
}


static unsigned int
wpdk_start_thread(void *arg)
{
	struct thread t;
	wpdk_thread_starting((struct thread *)arg, &t);
	wpdk_thread_done(t.index, (t.start)(t.arg));
	return 0;
}


int
wpdk_pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void *), void *arg)
{
	struct thread *pthread;
	unsigned int id;
	bool detached;
	int error;
	HANDLE h;

	if (!thread || !start_routine)
		return EINVAL;

	wpdk_set_invalid_handler();

	if ((pthread = wpdk_allocate_thread()) == NULL)
		return errno;

	pthread->start = start_routine;
	pthread->arg = arg;

	detached = (attr && attr->detachstate == PTHREAD_CREATE_DETACHED);

	h = (HANDLE)_beginthreadex(NULL, (attr ? attr->stacksize : 0),
					wpdk_start_thread, pthread, CREATE_SUSPENDED, &id);

	error = errno;
	wpdk_set_thread_info(pthread, id, h, detached);

	if (h == 0) return error;

	ResumeThread(h);
	if (detached) CloseHandle(h);

	// HACK - pthread_create - DPDK should be setting priority
	SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
	SetThreadPriority(h, THREAD_PRIORITY_TIME_CRITICAL);

	*thread = id;
	return 0;
}


int
wpdk_pthread_join(pthread_t thread, void **value_ptr)
{
	int index, error;
	bool detached;
	HANDLE h;
	DWORD rc;

	if (!thread || thread == GetCurrentThreadId())
		return EINVAL;

	if (!wpdk_get_thread_info(thread, &h, &index))
		return EINVAL;

	do rc = WaitForSingleObjectEx(h, INFINITE, TRUE);
	while (rc != WAIT_OBJECT_0 && rc != WAIT_FAILED);

	error = wpdk_last_error();
	detached = wpdk_detach_thread(thread, index, value_ptr);

	return (rc == WAIT_FAILED) ? error : detached ? 0 : ESRCH;
}


int
wpdk_pthread_detach(pthread_t thread)
{
	return wpdk_detach_thread(thread, -1, NULL) ? 0 : EINVAL;
}


void
wpdk_pthread_exit(void *value_ptr)
{
	wpdk_thread_done(-1, value_ptr);
	_endthreadex(0);
}


int
wpdk_pthread_cancel(pthread_t thread)
{
	if (!thread) return EINVAL;

	WPDK_UNIMPLEMENTED();
	return ENOSYS;
}


int
wpdk_pthread_setcancelstate(int state, int *oldstate)
{
	if (!oldstate) return EINVAL;

	switch (state) {
		case PTHREAD_CANCEL_ENABLE:
			WPDK_UNIMPLEMENTED();
			*oldstate = PTHREAD_CANCEL_DISABLE;
			return 0;

		case PTHREAD_CANCEL_DISABLE:
			*oldstate = PTHREAD_CANCEL_DISABLE;
			return 0;
	}

	return EINVAL;
}


int
wpdk_pthread_setcanceltype(int type, int *oldtype)
{
	if (!oldtype) return EINVAL;

	switch (type) {
		case PTHREAD_CANCEL_DEFERRED:
		case PTHREAD_CANCEL_ASYNCHRONOUS:
			*oldtype = PTHREAD_CANCEL_DEFERRED;
			return 0;
	}

	return EINVAL;
}


void
wpdk_pthread_testcancel(void)
{
	WPDK_UNIMPLEMENTED();
}


int
wpdk_pthread_setaffinity_np(pthread_t thread, size_t cpusetsize, const cpuset_t *cpuset)
{
	DWORD_PTR affinity;
	int error;
	HANDLE h;

	if (!thread || cpusetsize < sizeof(affinity) || !cpuset)
		return EINVAL;

	h = OpenThread(THREAD_SET_INFORMATION|THREAD_QUERY_INFORMATION,
			FALSE, thread);

	if (h == NULL) {
		wpdk_last_error();
		return errno;
	}

	affinity = SetThreadAffinityMask(h, cpuset->_bits[0]);
	error = wpdk_last_error();

	CloseHandle(h);
	return (affinity != 0) ? 0 : error;
}


/*
 *  Windows doesn't have GetThreadAffinityMask(), so change the
 *  affinity temporarily to find the current value.  Loop through the
 *  possible temporary affinities until we find one that succeeds.
 */
int
wpdk_pthread_getaffinity_np(pthread_t thread, size_t cpusetsize, cpuset_t *cpuset)
{
	DWORD_PTR affinity, temporary;
	int error;
	HANDLE h;

	if (!thread || cpusetsize < sizeof(affinity) || !cpuset)
		return EINVAL;

	h = OpenThread(THREAD_SET_INFORMATION|THREAD_QUERY_INFORMATION,
			FALSE, thread);

	if (h == NULL) {
		wpdk_last_error();
		return errno;
	}

	for (temporary = 1; temporary != 0; temporary <<= 1) {
		affinity = SetThreadAffinityMask(h, temporary);

		if (affinity != 0) {
			/* Reinstate the previous value */
			SetThreadAffinityMask(h, affinity);
			CloseHandle(h);

			memset(cpuset, 0, cpusetsize);
			cpuset->_bits[0] = affinity;
			return 0;
		}
	}

	error = wpdk_last_error();
	CloseHandle(h);
	return error;
}


int
wpdk_pthread_equal(pthread_t t1, pthread_t t2)
{
	return (t1 == t2);
}
