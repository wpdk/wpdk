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
typedef unsigned long pthread_t;
typedef unsigned long pthread_key_t;
typedef struct { intptr_t x; } pthread_cond_t;
typedef struct { intptr_t x[4]; } pthread_barrier_t;
typedef struct { intptr_t x[5]; } pthread_spinlock_t;
typedef struct { void *x; long y[2]; intptr_t z[3]; } pthread_mutex_t;

#define PTHREAD_COND_INITIALIZER	{0}
#define PTHREAD_MUTEX_INITIALIZER	{(void *)-1,{-1,0},{0,0,4000}}

#define PTHREAD_MUTEX_NORMAL		0
#define PTHREAD_MUTEX_ERRORCHECK	1
#define PTHREAD_MUTEX_RECURSIVE		2
#define PTHREAD_MUTEX_DEFAULT		PTHREAD_MUTEX_RECURSIVE

#define PTHREAD_MUTEX_STALLED		0
#define PTHREAD_MUTEX_ROBUST		1

#define PTHREAD_PROCESS_PRIVATE		0
#define PTHREAD_PROCESS_SHARED		1

#define PTHREAD_PRIO_NONE			0
#define PTHREAD_PRIO_INHERIT		1
#define PTHREAD_PRIO_PROTECT		2

#define PTHREAD_CREATE_JOINABLE		0
#define PTHREAD_CREATE_DETACHED		1

typedef struct pthread_mutexattr_s {
	int type;
	int robust;
	int pshared;
	int protocol;
} pthread_mutexattr_t;

int wpdk_pthread_mutexattr_init(pthread_mutexattr_t *attr);
int wpdk_pthread_mutexattr_destroy(pthread_mutexattr_t *attr);
int wpdk_pthread_mutexattr_gettype(const pthread_mutexattr_t *attr, int *type);
int wpdk_pthread_mutexattr_settype(pthread_mutexattr_t *attr, int type);
int wpdk_pthread_mutexattr_getrobust(const pthread_mutexattr_t *attr, int *robust);
int wpdk_pthread_mutexattr_setrobust(pthread_mutexattr_t *attr, int robust);
int wpdk_pthread_mutexattr_getpshared(const pthread_mutexattr_t *attr, int *pshared);
int wpdk_pthread_mutexattr_setpshared(pthread_mutexattr_t *attr, int pshared);
int wpdk_pthread_mutexattr_getprotocol(const pthread_mutexattr_t *attr, int *protocol);
int wpdk_pthread_mutexattr_setprotocol(pthread_mutexattr_t *attr, int protocol);
int wpdk_pthread_mutexattr_setprioceiling(pthread_mutexattr_t *attr, int prioceiling);
int wpdk_pthread_mutexattr_getprioceiling(const pthread_mutexattr_t *attr, int *prioceiling);

int wpdk_pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *mutexattr);
int wpdk_pthread_mutex_lock(pthread_mutex_t *mutex);
int wpdk_pthread_mutex_trylock(pthread_mutex_t *mutex);
int wpdk_pthread_mutex_unlock(pthread_mutex_t *mutex);
int wpdk_pthread_mutex_destroy(pthread_mutex_t *mutex);
int wpdk_pthread_mutex_consistent(pthread_mutex_t *mutex);

#ifndef _WPDK_BUILD_LIB_
static inline int pthread_mutexattr_init(pthread_mutexattr_t *attr) {
	return wpdk_pthread_mutexattr_init(attr);
}
#define pthread_mutexattr_destroy(attr) wpdk_pthread_mutexattr_destroy(attr)
#define pthread_mutexattr_gettype(attr,type) wpdk_pthread_mutexattr_gettype(attr,type)
#define pthread_mutexattr_settype(attr,type) wpdk_pthread_mutexattr_settype(attr,type)
#define pthread_mutexattr_getrobust(attr,robust) wpdk_pthread_mutexattr_getrobust(attr,robust)
#define pthread_mutexattr_setrobust(attr,robust) wpdk_pthread_mutexattr_setrobust(attr,robust)
#define pthread_mutexattr_getpshared(attr,pshared) wpdk_pthread_mutexattr_getpshared(attr,pshared)
#define pthread_mutexattr_setpshared(attr,pshared) wpdk_pthread_mutexattr_setpshared(attr,pshared)
#define pthread_mutexattr_getprotocol(attr,protocol) wpdk_pthread_mutexattr_getprotocol(attr,protocol)
#define pthread_mutexattr_setprotocol(attr,protocol) wpdk_pthread_mutexattr_setprotocol(attr,protocol)
#define pthread_mutexattr_setprioceiling(attr,prio) wpdk_pthread_mutexattr_setprioceiling(attr,prio)
#define pthread_mutexattr_getprioceiling(attr,prio) wpdk_pthread_mutexattr_getprioceiling(attr,prio)
static inline int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *mutexattr) {
	return wpdk_pthread_mutex_init(mutex, mutexattr);
}
#define pthread_mutex_lock(mutex) wpdk_pthread_mutex_lock(mutex)
#define pthread_mutex_trylock(mutex) wpdk_pthread_mutex_trylock(mutex)
#define pthread_mutex_unlock(mutex) wpdk_pthread_mutex_unlock(mutex)
#define pthread_mutex_destroy(mutex) wpdk_pthread_mutex_destroy(mutex)
#define pthread_mutex_consistent(mutex) wpdk_pthread_mutex_consistent(mutex)
#endif

int wpdk_pthread_spin_destroy(pthread_spinlock_t *lock);
int wpdk_pthread_spin_init(pthread_spinlock_t *lock, int pshared);
int wpdk_pthread_spin_lock(pthread_spinlock_t *lock);
int wpdk_pthread_spin_trylock(pthread_spinlock_t *lock);
int wpdk_pthread_spin_unlock(pthread_spinlock_t *lock);

#ifndef _WPDK_BUILD_LIB_
#define pthread_spin_destroy(lock) wpdk_pthread_spin_destroy(lock)
#define pthread_spin_init(lock,pshared) wpdk_pthread_spin_init(lock,pshared)
#define pthread_spin_lock(lock) wpdk_pthread_spin_lock(lock)
#define pthread_spin_trylock(lock) wpdk_pthread_spin_trylock(lock)
#define pthread_spin_unlock(lock) wpdk_pthread_spin_unlock(lock)
#endif

typedef struct { int pshared; } pthread_barrierattr_t;

#define PTHREAD_BARRIER_SERIAL_THREAD	1

int wpdk_pthread_barrierattr_destroy(pthread_barrierattr_t *attr);
int wpdk_pthread_barrierattr_init(pthread_barrierattr_t *attr);
int wpdk_pthread_barrierattr_getpshared(const pthread_barrierattr_t *attr, int *pshared);
int wpdk_pthread_barrierattr_setpshared(pthread_barrierattr_t *attr, int pshared);
int wpdk_pthread_barrier_destroy(pthread_barrier_t *barrier);
int wpdk_pthread_barrier_init(pthread_barrier_t *barrier,
		const pthread_barrierattr_t *attr, unsigned count);
int wpdk_pthread_barrier_wait(pthread_barrier_t *barrier);

#ifndef _WPDK_BUILD_LIB_
#define pthread_barrierattr_destroy(attr) wpdk_pthread_barrierattr_destroy(attr)
#define pthread_barrierattr_init(attr) wpdk_pthread_barrierattr_init(attr)
#define pthread_barrierattr_getpshared(attr,pshared) wpdk_pthread_barrierattr_getpshared(attr,pshared)
#define pthread_barrierattr_setpshared(attr,pshared) wpdk_pthread_barrierattr_setpshared(attr,pshared)
#define pthread_barrier_destroy(barrier) wpdk_pthread_barrier_destroy(barrier)
#define pthread_barrier_init(barrier,attr,count) wpdk_pthread_barrier_init(barrier,attr,count)
#define pthread_barrier_wait(barrier) wpdk_pthread_barrier_wait(barrier)
#endif

typedef struct { int pshared; } pthread_condattr_t;

int wpdk_pthread_condattr_init(pthread_condattr_t *attr);
int wpdk_pthread_condattr_destroy(pthread_condattr_t *attr);
int wpdk_pthread_condattr_getpshared(const pthread_condattr_t *attr, int *pshared);
int wpdk_pthread_condattr_setpshared(pthread_condattr_t *attr, int pshared);
int wpdk_pthread_cond_init(pthread_cond_t *cond, const pthread_condattr_t *attr);
int wpdk_pthread_cond_destroy(pthread_cond_t *cond);
int wpdk_pthread_cond_signal(pthread_cond_t *cond);
int wpdk_pthread_cond_broadcast(pthread_cond_t *cond);
int wpdk_pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);
int wpdk_pthread_cond_timedwait(pthread_cond_t *cond, pthread_mutex_t *mutex, const struct timespec *abstime);

#ifndef _WPDK_BUILD_LIB_
#define pthread_condattr_init(attr) wpdk_pthread_condattr_init(attr)
#define pthread_condattr_destroy(attr) wpdk_pthread_condattr_destroy(attr)
#define pthread_condattr_getpshared(attr,pshared) wpdk_pthread_condattr_getpshared(attr,pshared)
#define pthread_condattr_setpshared(attr,pshared) wpdk_pthread_condattr_setpshared(attr,pshared)
#define pthread_cond_init(cond,attr) wpdk_pthread_cond_init(cond,attr)
#define pthread_cond_destroy(cond) wpdk_pthread_cond_destroy(cond)
#define pthread_cond_signal(cond) wpdk_pthread_cond_signal(cond)
#define pthread_cond_broadcast(cond) wpdk_pthread_cond_broadcast(cond)
#define pthread_cond_wait(cond,mutex) wpdk_pthread_cond_wait(cond,mutex)
#define pthread_cond_timedwait(cond,mutex,abstime) wpdk_pthread_cond_timedwait(cond,mutex,abstime)
#endif

typedef struct pthread_attr_s {
	int detachstate;
	unsigned int stacksize;
} pthread_attr_t;

int wpdk_pthread_attr_init(pthread_attr_t *attr);
int wpdk_pthread_attr_destroy(pthread_attr_t *attr);
int wpdk_pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate);
int wpdk_pthread_attr_getdetachstate(const pthread_attr_t *attr, int *detachstate);
int wpdk_pthread_attr_setstacksize(pthread_attr_t *attr, size_t stacksize);
int wpdk_pthread_attr_getstacksize(const pthread_attr_t *attr, size_t *stacksize);
pthread_t wpdk_pthread_self(void);
int wpdk_pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void*), void *arg);
int wpdk_pthread_join(pthread_t thread, void **value_ptr);
int wpdk_pthread_detach(pthread_t thread);
int wpdk_pthread_cancel(pthread_t thread);
void wpdk_pthread_exit(void *value_ptr);

#ifndef _WPDK_BUILD_LIB_
#define pthread_attr_init(attr) wpdk_pthread_attr_init(attr)
#define pthread_attr_destroy(attr) wpdk_pthread_attr_destroy(attr)
#define pthread_attr_setdetachstate(attr,detachstate) wpdk_pthread_attr_setdetachstate(attr,detachstate)
#define pthread_attr_getdetachstate(attr,detachstate) wpdk_pthread_attr_getdetachstate(attr,detachstate)
#define pthread_attr_setstacksize(attr,stacksize) wpdk_pthread_attr_setstacksize(attr,stacksize)
#define pthread_attr_getstacksize(attr,stacksize) wpdk_pthread_attr_getstacksize(attr,stacksize)
#define pthread_self() wpdk_pthread_self()
#define pthread_create(thread,attr,start,arg) wpdk_pthread_create(thread,attr,start,arg)
#define pthread_join(thread,ptr) wpdk_pthread_join(thread,ptr)
#define pthread_detach(thread) wpdk_pthread_detach(thread)
#define pthread_cancel(thread) wpdk_pthread_cancel(thread)
#define pthread_exit(ptr) wpdk_pthread_exit(ptr)
#endif

#define PTHREAD_CANCEL_DISABLE	0
#define PTHREAD_CANCEL_ENABLE	1

#define PTHREAD_CANCEL_DEFERRED		0
#define PTHREAD_CANCEL_ASYNCHRONOUS	1 

int wpdk_pthread_setcancelstate(int state, int *oldstate);
int wpdk_pthread_setcanceltype(int type, int *oldtype);
void wpdk_pthread_testcancel(void);
int wpdk_pthread_setaffinity_np(pthread_t thread, size_t cpusetsize, const cpuset_t *cpuset);
int wpdk_pthread_getaffinity_np(pthread_t thread, size_t cpusetsize, cpuset_t *cpuset);
int wpdk_pthread_equal(pthread_t t1, pthread_t t2);

#ifndef _WPDK_BUILD_LIB_
#define pthread_setcancelstate(state,oldstate) wpdk_pthread_setcancelstate(state,oldstate)
#define pthread_setcanceltype(type,oldtype) wpdk_pthread_setcanceltype(type,oldtype)
#define pthread_testcancel() wpdk_pthread_testcancel()
#define pthread_setaffinity_np(thread,cpusetsize,cpuset) wpdk_pthread_setaffinity_np(thread,cpusetsize,cpuset)
#define pthread_getaffinity_np(thread,cpusetsize,cpuset) wpdk_pthread_getaffinity_np(thread,cpusetsize,cpuset)
#define pthread_equal(t1,t2) wpdk_pthread_equal(t1,t2)
#endif

int wpdk_pthread_key_create(pthread_key_t *key, void (*destructor)(void*));
int wpdk_pthread_key_delete(pthread_key_t key);
void *wpdk_pthread_getspecific(pthread_key_t key);
int wpdk_pthread_setspecific(pthread_key_t key, const void *value);

#ifndef _WPDK_BUILD_LIB_
#define pthread_key_create(key,destructor) wpdk_pthread_key_create(key,destructor)
#define pthread_key_delete(key) wpdk_pthread_key_delete(key)
#define pthread_getspecific(key) wpdk_pthread_getspecific(key)
#define pthread_setspecific(key,value) wpdk_pthread_setspecific(key,value)
#endif

void wpdk_pthread_set_name_np(pthread_t thread, const char *name);
void wpdk_pthread_get_name_np(pthread_t thread, char *name, size_t len);
int wpdk_pthread_setname_np(pthread_t thread, const char *name);
int wpdk_pthread_getname_np(pthread_t thread, char *name, size_t len);

#ifndef _WPDK_BUILD_LIB_
#define pthread_set_name_np(thread,name) wpdk_pthread_setname_np(thread,name)
#define pthread_get_name_np(thread,name,len) wpdk_pthread_getname_np(thread,name,len)
#define pthread_setname_np(thread,name) wpdk_pthread_setname_np(thread,name)
#define pthread_getname_np(thread,name,len) wpdk_pthread_getname_np(thread,name,len)
#endif

_WPDK_END_C_HEADER

#endif /* _WPDK_PTHREAD_H_ */
