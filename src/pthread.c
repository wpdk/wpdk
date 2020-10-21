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
#include <pthread.h>


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
		InterlockedExchangePointer(&lock->DebugInfo, (void *)-1);
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
	return 0;
}


int
wpdk_pthread_attr_destroy(pthread_attr_t *attr)
{
	if (!attr) return EINVAL;
	return 0;
}


pthread_t
wpdk_pthread_self()
{
	// HACK - pthread_self() is only used to get self id to change name
	return GetCurrentThread();
}


int
wpdk_pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void *), void *arg)
{
	HANDLE hThread;

	// HACK - pthread_create check implementation
	// HACK - pthread_create should use beginthreadex if using CRT

	UNREFERENCED_PARAMETER(attr);

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-function-type"
#endif
	// HACK - return code from start_routine is DWORD, not 'void *'
	hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)start_routine, arg, 0, (LPDWORD)thread);
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

	if (hThread == NULL) {
		// HACK - pthread_create error code
		return EINVAL;
	}

	SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
	SetThreadPriority(hThread, THREAD_PRIORITY_TIME_CRITICAL);
	*thread = hThread;
	return 0;
}


int
wpdk_pthread_join(pthread_t thread, void **value_ptr)
{
	DWORD rc, code;

	// HACK - pthread_exit - check for errors

	do rc = WaitForSingleObjectEx(thread, INFINITE, TRUE);
	while (rc != WAIT_OBJECT_0 && rc != WAIT_FAILED);

	if (value_ptr)
		*value_ptr = (GetExitCodeThread(thread, &code) != 0) ? (void *)(ULONG_PTR)code : 0;

	CloseHandle(thread);
	return 0;
}


int
wpdk_pthread_detach(pthread_t thread)
{
	// HACK - unimplemented
	CloseHandle(thread);
	return EINVAL;
}


int
wpdk_pthread_cancel(pthread_t thread)
{
	// HACK - unimplemented
	UNREFERENCED_PARAMETER(thread);
	WPDK_UNIMPLEMENTED();
	return EINVAL;
}


void
wpdk_pthread_exit(void *value_ptr)
{
	// HACK - pthread_exit unimplemented
	// HACK - size of pthread_exit return code
	ExitThread((DWORD)(ULONG_PTR)value_ptr);
}


int
wpdk_pthread_setcancelstate(int state, int *oldstate)
{
	// HACK - unimplemented
	if (!oldstate || state < PTHREAD_CANCEL_DISABLE || state > PTHREAD_CANCEL_ENABLE)
		return EINVAL;

	*oldstate = PTHREAD_CANCEL_DISABLE;
	return 0;
}


int
wpdk_pthread_setcanceltype(int type, int *oldtype)
{
	// HACK - unimplemented
	if (!oldtype || type < PTHREAD_CANCEL_DEFERRED || type > PTHREAD_CANCEL_ASYNCHRONOUS)
		return EINVAL;

	*oldtype = PTHREAD_CANCEL_DEFERRED;
	return 0;
}


void
wpdk_pthread_testcancel(void)
{
	// HACK - unimplemented
	WPDK_UNIMPLEMENTED();
}


int
wpdk_pthread_setaffinity_np(pthread_t thread, size_t cpusetsize, const cpuset_t *cpuset)
{
	DWORD_PTR affinity;

	if (!thread || cpusetsize < sizeof(affinity) || !cpuset)
		return EINVAL;

	affinity = SetThreadAffinityMask(thread, cpuset->_bits[0]);

	// HACK - error code
	return (affinity != 0) ? 0 : EINVAL;
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

	if (!thread || cpusetsize < sizeof(affinity) || !cpuset)
		return EINVAL;

	for (temporary = 1; temporary != 0; temporary <<= 1) {
		affinity = SetThreadAffinityMask(thread, temporary);

		if (affinity != 0) {
			/* Reinstate the previous value */
			SetThreadAffinityMask(thread, affinity);

			memset(cpuset, 0, cpusetsize);
			cpuset->_bits[0] = affinity;
			return 0;
		}
	}

	// HACK - error code
	return EINVAL;
}


int
wpdk_pthread_equal(pthread_t t1, pthread_t t2)
{
	return (t1 == t2);
}
