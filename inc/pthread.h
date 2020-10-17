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

#ifndef _WPDK_PTHREAD_H_
#define _WPDK_PTHREAD_H_

#include <wpdk/header.h>
#include <sched.h>
#include <time.h>

_WPDK_BEGIN_C_HEADER

/*
 *  Define structures which are equivalent to the underlying
 *  implementation, but which avoid the need to include windows.h.
 */
typedef void *pthread_t;
typedef struct { intptr_t x; } pthread_cond_t;
typedef struct { intptr_t x[4]; } pthread_barrier_t;
typedef struct { intptr_t x[5]; } pthread_spinlock_t;
typedef struct { void *x; long y[2]; void *z[3]; } pthread_mutex_t;

#define PTHREAD_COND_INITIALIZER	{0}
#define PTHREAD_MUTEX_INITIALIZER	{(void *)-1,{-1}}

#define PTHREAD_MUTEX_NORMAL		0
#define PTHREAD_MUTEX_ERRORCHECK	1
#define PTHREAD_MUTEX_RECURSIVE		2
#define PTHREAD_MUTEX_DEFAULT		PTHREAD_MUTEX_NORMAL

#define PTHREAD_MUTEX_STALLED		0
#define PTHREAD_MUTEX_ROBUST		1

#define PTHREAD_PROCESS_PRIVATE		0
#define PTHREAD_PROCESS_SHARED		1

typedef struct pthread_mutexattr_s {
	int type;
	int robust;
	int pshared;
} pthread_mutexattr_t;

int pthread_mutexattr_destroy(pthread_mutexattr_t *attr);
int pthread_mutexattr_gettype(const pthread_mutexattr_t *attr, int *type);
int pthread_mutexattr_settype(pthread_mutexattr_t *attr, int type);
int pthread_mutexattr_getrobust(const pthread_mutexattr_t *attr, int *robust);
int pthread_mutexattr_setrobust(pthread_mutexattr_t *attr, int robust);
int pthread_mutexattr_getpshared(const pthread_mutexattr_t *attr, int *pshared);
int pthread_mutexattr_setpshared(pthread_mutexattr_t *attr, int pshared);

int pthread_mutex_lock(pthread_mutex_t *mutex);
int pthread_mutex_trylock(pthread_mutex_t *mutex);
int pthread_mutex_unlock(pthread_mutex_t *mutex);
int pthread_mutex_destroy(pthread_mutex_t *mutex);
int pthread_mutex_consistent(pthread_mutex_t *mutex);

int pthread_spin_destroy(pthread_spinlock_t *lock);
int pthread_spin_init(pthread_spinlock_t *lock, int pshared);
int pthread_spin_lock(pthread_spinlock_t *lock);
int pthread_spin_trylock(pthread_spinlock_t *lock);
int pthread_spin_unlock(pthread_spinlock_t *lock);

typedef struct { int pshared; } pthread_barrierattr_t;

int pthread_barrierattr_destroy(pthread_barrierattr_t *attr);
int pthread_barrierattr_init(pthread_barrierattr_t *attr);
int pthread_barrierattr_getpshared(const pthread_barrierattr_t *attr, int *pshared);
int pthread_barrierattr_setpshared(pthread_barrierattr_t *attr, int pshared);

#define PTHREAD_BARRIER_SERIAL_THREAD	1

int pthread_barrier_destroy(pthread_barrier_t *barrier);
int pthread_barrier_init(pthread_barrier_t *barrier,
		const pthread_barrierattr_t *attr, unsigned count);
int pthread_barrier_wait(pthread_barrier_t *barrier);	

typedef struct { int pshared; } pthread_condattr_t;

int pthread_condattr_init(pthread_condattr_t *attr);
int pthread_condattr_destroy(pthread_condattr_t *attr);
int pthread_condattr_getpshared(const pthread_condattr_t *attr, int *pshared);
int pthread_condattr_setpshared(pthread_condattr_t *attr, int pshared);

int pthread_cond_init(pthread_cond_t *cond, const pthread_condattr_t *attr);
int pthread_cond_destroy(pthread_cond_t *cond);
int pthread_cond_signal(pthread_cond_t *cond);
int pthread_cond_broadcast(pthread_cond_t *cond);
int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);
int pthread_cond_timedwait(pthread_cond_t *cond, pthread_mutex_t *mutex, const struct timespec *abstime);

typedef struct pthread_attr_s {
	void *x;
} pthread_attr_t;

int pthread_attr_init(pthread_attr_t *attr);
int pthread_attr_destroy(pthread_attr_t *attr);

pthread_t pthread_self(void);
int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void*), void *arg);
int pthread_join(pthread_t thread, void **value_ptr);
int pthread_detach(pthread_t);
int pthread_cancel(pthread_t);
void pthread_exit(void *value_ptr);

#define PTHREAD_CANCEL_DISABLE	0
#define PTHREAD_CANCEL_ENABLE	1

#define PTHREAD_CANCEL_DEFERRED		0
#define PTHREAD_CANCEL_ASYNCHRONOUS	1 

int pthread_setcancelstate(int state, int *oldstate);
int pthread_setcanceltype(int type, int *oldtype);
void pthread_testcancel(void);

#if defined(_MSC_VER) && !defined(_WPDK_BUILD_LIB_)
#define wpdk_pthread_mutexattr_init __wrap_pthread_mutexattr_init
#define wpdk_pthread_mutex_init __wrap_pthread_mutex_init

int wpdk_pthread_mutexattr_init(pthread_mutexattr_t *attr);
int wpdk_pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *mutexattr);

#pragma comment(linker, "/alternatename:__wrap_pthread_mutex_init=wpdk_pthread_mutex_init")
#pragma comment(linker, "/alternatename:__real_pthread_mutex_init=wpdk_pthread_mutex_init")
#pragma comment(linker, "/alternatename:__wrap_pthread_mutexattr_init=wpdk_pthread_mutexattr_init")
#pragma comment(linker, "/alternatename:__real_pthread_mutexattr_init=wpdk_pthread_mutexattr_init")

static inline int pthread_mutexattr_init(pthread_mutexattr_t *attr) {
	return wpdk_pthread_mutexattr_init(attr);
}

static inline int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *mutexattr) {
	return wpdk_pthread_mutex_init(mutex, mutexattr);
}
#else
int pthread_mutexattr_init(pthread_mutexattr_t *attr);
int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *mutexattr);
#endif

int pthread_setaffinity_np(pthread_t thread, size_t cpusetsize, const cpuset_t *cpuset);
int pthread_getaffinity_np(pthread_t thread, size_t cpusetsize, cpuset_t *cpuset);
int pthread_equal(pthread_t t1, pthread_t t2);

_WPDK_END_C_HEADER

#endif /* _WPDK_PTHREAD_H_ */
