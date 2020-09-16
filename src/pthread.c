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

#include <wpdklib.h>
#include <pthread.h>


int __real_pthread_mutexattr_init(pthread_mutexattr_t *attr)
{
	if (!attr) return EINVAL;

	attr->type = PTHREAD_MUTEX_DEFAULT;
	attr->robust = PTHREAD_MUTEX_STALLED;
	attr->pshared = PTHREAD_PROCESS_PRIVATE;
	return 0;
}


int pthread_mutexattr_destroy(pthread_mutexattr_t *attr)
{
	if (!attr) return EINVAL;

	return 0;
}


int pthread_mutexattr_gettype(const pthread_mutexattr_t *attr, int *type)
{
	if (!attr || !type)
		return EINVAL;

	*type = attr->type;
	return 0;
}


int pthread_mutexattr_settype(pthread_mutexattr_t *attr, int type)
{
	if (!attr || type < PTHREAD_MUTEX_NORMAL || type > PTHREAD_MUTEX_RECURSIVE)
		return EINVAL;

	attr->type = type;
	return 0;
}


int pthread_mutexattr_getrobust(const pthread_mutexattr_t *attr, int *robust)
{
	if (!attr || !robust)
		return EINVAL;

	*robust = attr->robust;
	return 0;
}


int pthread_mutexattr_setrobust(pthread_mutexattr_t *attr, int robust)
{
	if (!attr || robust < PTHREAD_MUTEX_STALLED || robust > PTHREAD_MUTEX_ROBUST)
		return EINVAL;

	attr->robust = robust;
	return 0;
}


int pthread_mutexattr_getpshared(const pthread_mutexattr_t *attr, int *pshared)
{
	if (!attr || !pshared)
		return EINVAL;

	*pshared = attr->pshared;
	return 0;
}


int pthread_mutexattr_setpshared(pthread_mutexattr_t *attr, int pshared)
{
	if (!attr || pshared < PTHREAD_PROCESS_PRIVATE || pshared > PTHREAD_PROCESS_SHARED)
		return EINVAL;

	attr->pshared = pshared;
	return 0;
}


int __real_pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *mutexattr)
{
	if (!mutex) return EINVAL;

	if (mutexattr && (mutexattr->type < PTHREAD_MUTEX_NORMAL || mutexattr->type > PTHREAD_MUTEX_RECURSIVE))
		return EINVAL;

	// HACK - and spin count!
	InitializeCriticalSection(&mutex->lock);
	return 0;
}


int pthread_mutex_destroy(pthread_mutex_t *mutex)
{
	if (!mutex) return EINVAL;

	// HACK - for now this needs to be a no-op to get unit tests running
	// HACK revisit with extensive pthreads checking code

	// DeleteCriticalSection(&mutex->lock);
	return 0;
}


int pthread_mutex_lock(pthread_mutex_t *mutex)
{
	if (!mutex) return EINVAL;

	// HACK - NASTY to get unit tests running
	if (!mutex->lock.DebugInfo)
		InitializeCriticalSection(&mutex->lock);

	EnterCriticalSection(&mutex->lock);
	return 0;
}


int pthread_mutex_trylock(pthread_mutex_t *mutex)
{
	if (!mutex) return EINVAL;

	if (TryEnterCriticalSection(&mutex->lock) == 0)
		return EBUSY;

	return 0;
}


int pthread_mutex_unlock(pthread_mutex_t *mutex)
{
	if (!mutex) return EINVAL;

	LeaveCriticalSection(&mutex->lock);
	return 0;
}


int pthread_mutex_consistent(pthread_mutex_t *mutex)
{
	//HACK - not implemented
	WPDK_UNIMPLEMENTED();
	
	UNREFERENCED_PARAMETER(mutex);
	return 0;
}


int pthread_spin_init(pthread_spinlock_t *lock, int pshared)
{
	if (!lock || pshared < PTHREAD_PROCESS_PRIVATE || pshared > PTHREAD_PROCESS_SHARED)
		return EINVAL;

	// HACK - will eventually block
	InitializeCriticalSectionAndSpinCount(&lock->lock, 32000);
	return 0;
}


int pthread_spin_destroy(pthread_spinlock_t *lock)
{
	if (!lock) return EINVAL;

	// HACK - leave this out for now to avoid reuse after destroy issues
	// DeleteCriticalSection(&lock->lock);
	return 0;
}


int pthread_spin_lock(pthread_spinlock_t *lock)
{
	if (!lock) return EINVAL;

	EnterCriticalSection(&lock->lock);
	return 0;
}


int pthread_spin_trylock(pthread_spinlock_t *lock)
{
	if (!lock) return EINVAL;

	if (TryEnterCriticalSection(&lock->lock) == 0)
		return EBUSY;

	return 0;
}


int pthread_spin_unlock(pthread_spinlock_t *lock)
{
	if (!lock) return EINVAL;

	LeaveCriticalSection(&lock->lock);
	return 0;
}


int pthread_condattr_init(pthread_condattr_t *attr)
{
	if (!attr) return EINVAL;

	attr->pshared = PTHREAD_PROCESS_PRIVATE;
	return 0;
}


int pthread_condattr_destroy(pthread_condattr_t *attr)
{
	if (!attr) return EINVAL;
	return 0;
}


int pthread_condattr_getpshared(const pthread_condattr_t *attr, int *pshared)
{
	if (!attr || !pshared)
		return EINVAL;
	*pshared = attr->pshared;
	return 0;
}


int pthread_condattr_setpshared(pthread_condattr_t *attr, int pshared)
{
	if (!attr || pshared < PTHREAD_PROCESS_PRIVATE || pshared > PTHREAD_PROCESS_SHARED)
		return EINVAL;

	attr->pshared = pshared;
	return 0;
}


int pthread_cond_init(pthread_cond_t *cond, const pthread_condattr_t *attr)
{
	UNREFERENCED_PARAMETER(attr);

	if (!cond) return EINVAL;

	InitializeConditionVariable(&cond->cond);
	return 0;
}


int pthread_cond_destroy(pthread_cond_t *cond)
{
	UNREFERENCED_PARAMETER(cond);
	return 0;
}


int pthread_cond_signal(pthread_cond_t *cond)
{
	WakeConditionVariable(&cond->cond);
	return 0;
}


int pthread_cond_broadcast(pthread_cond_t *cond)
{
	WakeAllConditionVariable(&cond->cond);
	return 0;
}


int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex)
{
	SleepConditionVariableCS(&cond->cond, &mutex->lock, INFINITE);
	return 0;
}


int pthread_cond_timedwait(pthread_cond_t *cond, pthread_mutex_t *mutex, const struct timespec *abstime)
{
	// HACK - implementation
	UNREFERENCED_PARAMETER(abstime);
	
	return pthread_cond_wait(cond, mutex);
}


int pthread_attr_init(pthread_attr_t *attr)
{
	if (!attr) return EINVAL;
	return 0;
}


int pthread_attr_destroy(pthread_attr_t *attr)
{
	if (!attr) return EINVAL;
	return 0;
}


pthread_t pthread_self()
{
	// HACK - pthread_self() is only used to get self id to change name
	return GetCurrentThread();
}


int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void *), void *arg)
{
	// HACK - pthread_create check implementation
	// HACK - pthread_create should use beginthreadex if using CRT

	UNREFERENCED_PARAMETER(attr);

	HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)start_routine, arg, 0, (LPDWORD)thread);

	if (hThread == NULL) {
		// HACK - pthread_create error code
		return EINVAL;
	}

	SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
	SetThreadPriority(hThread, THREAD_PRIORITY_TIME_CRITICAL);
	*thread = hThread;
	return 0;
}


int pthread_join(pthread_t thread, void **value_ptr)
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


int pthread_detach(pthread_t thread)
{
	// HACK - unimplemented
	CloseHandle(thread);
	return EINVAL;
}


int pthread_cancel(pthread_t thread)
{
	// HACK - unimplemented
	UNREFERENCED_PARAMETER(thread);
	WPDK_UNIMPLEMENTED();
	return EINVAL;
}


void pthread_exit(void *value_ptr)
{
	// HACK - pthread_exit unimplemented
	// HACK - size of pthread_exit return code
	ExitThread((DWORD)(ULONG_PTR)value_ptr);
}


int pthread_setcancelstate(int state, int *oldstate)
{
	// HACK - unimplemented
	if (!oldstate || state < PTHREAD_CANCEL_DISABLE || state > PTHREAD_CANCEL_ENABLE)
		return EINVAL;

	*oldstate = PTHREAD_CANCEL_DISABLE;
	return 0;
}


int pthread_setcanceltype(int type, int *oldtype)
{
	// HACK - unimplemented
	if (!oldtype || type < PTHREAD_CANCEL_DEFERRED || type > PTHREAD_CANCEL_ASYNCHRONOUS)
		return EINVAL;

	*oldtype = PTHREAD_CANCEL_DEFERRED;
	return 0;
}


void pthread_testcancel(void)
{
	// HACK - unimplemented
	WPDK_UNIMPLEMENTED();
}
