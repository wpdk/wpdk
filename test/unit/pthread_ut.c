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
 */

#include <wpdk/internal.h>
#include <sys/time.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#include <CUnit/Basic.h>


static int
null_init(void)
{
	return 0;
}


static int
null_clean(void)
{
	return 0;
}


static void
test_mutexattr_type(void)
{
	pthread_mutexattr_t attr;
	int rc, val;

	/* Check initialisation */
	rc = pthread_mutexattr_init(&attr);
	CU_ASSERT(rc == 0);

	/* Check type */
	val = -1;
	rc = wpdk_pthread_mutexattr_gettype(&attr, &val);
	CU_ASSERT(rc == 0);
	CU_ASSERT(val == PTHREAD_MUTEX_DEFAULT);

	/* Check gettype null attr */
	val = -1;
	rc = wpdk_pthread_mutexattr_gettype(NULL, &val);
	CU_ASSERT(rc == EINVAL);

	/* Check gettype null arg */
	rc = wpdk_pthread_mutexattr_gettype(&attr, NULL);
	CU_ASSERT(rc == EINVAL);

	/* Check settype */
	val = -1;
	rc = wpdk_pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_NORMAL);
	CU_ASSERT(rc == 0);
	rc = wpdk_pthread_mutexattr_gettype(&attr, &val);
	CU_ASSERT(rc == 0);
	CU_ASSERT(val == PTHREAD_MUTEX_NORMAL);

	/* Check settype null attr */
	rc = wpdk_pthread_mutexattr_settype(NULL, PTHREAD_MUTEX_NORMAL);
	CU_ASSERT(rc == EINVAL);

	/* Check invalid type */
	rc = wpdk_pthread_mutexattr_settype(&attr, -1);
	CU_ASSERT(rc == EINVAL);
}


static void
test_mutexattr_pshared(void)
{
	pthread_mutexattr_t attr;
	int rc, val;

	/* Check initialisation */
	rc = pthread_mutexattr_init(&attr);
	CU_ASSERT(rc == 0);

	/* Check pshared */
	val = -1;
	rc = wpdk_pthread_mutexattr_getpshared(&attr, &val);
	CU_ASSERT(rc == 0);
	CU_ASSERT(val == PTHREAD_PROCESS_PRIVATE);

	/* Check getpshared null attr */
	val = -1;
	rc = wpdk_pthread_mutexattr_getpshared(NULL, &val);
	CU_ASSERT(rc == EINVAL);

	/* Check getpshared null arg */
	rc = wpdk_pthread_mutexattr_getpshared(&attr, NULL);
	CU_ASSERT(rc == EINVAL);

	/* Check setpshared */
	val = -1;
	rc = wpdk_pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
	CU_ASSERT(rc == 0);
	rc = wpdk_pthread_mutexattr_getpshared(&attr, &val);
	CU_ASSERT(rc == 0);
	CU_ASSERT(val == PTHREAD_PROCESS_SHARED);

	/* Check setpshared null attr */
	rc = wpdk_pthread_mutexattr_setpshared(NULL, PTHREAD_PROCESS_SHARED);
	CU_ASSERT(rc == EINVAL);

	/* Check invalid pshared */
	rc = wpdk_pthread_mutexattr_setpshared(&attr, -1);
	CU_ASSERT(rc == EINVAL);
}


static void
test_mutexattr_robust(void)
{
	pthread_mutexattr_t attr;
	int rc, val;

	/* Check initialisation */
	rc = pthread_mutexattr_init(&attr);
	CU_ASSERT(rc == 0);

	/* Check robust */
	val = -1;
	rc = wpdk_pthread_mutexattr_getrobust(&attr, &val);
	CU_ASSERT(rc == 0);
	CU_ASSERT(val == PTHREAD_MUTEX_STALLED);

	/* Check getrobust null attr */
	val = -1;
	rc = wpdk_pthread_mutexattr_getrobust(NULL, &val);
	CU_ASSERT(rc == EINVAL);

	/* Check getrobust null arg */
	rc = wpdk_pthread_mutexattr_getrobust(&attr, NULL);
	CU_ASSERT(rc == EINVAL);

	/* Check setrobust */
	val = -1;
	rc = wpdk_pthread_mutexattr_setrobust(&attr, PTHREAD_MUTEX_ROBUST);
	CU_ASSERT(rc == 0);
	rc = wpdk_pthread_mutexattr_getrobust(&attr, &val);
	CU_ASSERT(rc == 0);
	CU_ASSERT(val == PTHREAD_MUTEX_ROBUST);

	/* Check setrobust null attr */
	rc = wpdk_pthread_mutexattr_setrobust(NULL, PTHREAD_MUTEX_ROBUST);
	CU_ASSERT(rc == EINVAL);

	/* Check invalid robust */
	rc = wpdk_pthread_mutexattr_setrobust(&attr, -1);
	CU_ASSERT(rc == EINVAL);
}


static void
test_mutexattr_protocol(void)
{
	pthread_mutexattr_t attr;
	int rc, val;

	/* Check initialisation */
	rc = pthread_mutexattr_init(&attr);
	CU_ASSERT(rc == 0);

	/* Check protocol */
	val = -1;
	rc = wpdk_pthread_mutexattr_getprotocol(&attr, &val);
	CU_ASSERT(rc == 0);
	CU_ASSERT(val == PTHREAD_PRIO_NONE);

	/* Check getprotocol null attr */
	val = -1;
	rc = wpdk_pthread_mutexattr_getprotocol(NULL, &val);
	CU_ASSERT(rc == EINVAL);

	/* Check getprotocol null arg */
	rc = wpdk_pthread_mutexattr_getprotocol(&attr, NULL);
	CU_ASSERT(rc == EINVAL);

	/* Check setprotocol */
	val = -1;
	rc = wpdk_pthread_mutexattr_setprotocol(&attr, PTHREAD_PRIO_INHERIT);
	CU_ASSERT(rc == ENOSYS);
	rc = wpdk_pthread_mutexattr_getprotocol(&attr, &val);
	CU_ASSERT(rc == 0);
	CU_ASSERT(val == PTHREAD_PRIO_NONE);

	/* Check setprotocol null attr */
	rc = wpdk_pthread_mutexattr_setprotocol(NULL, PTHREAD_PRIO_NONE);
	CU_ASSERT(rc == EINVAL);

	/* Check invalid protocol */
	rc = wpdk_pthread_mutexattr_setprotocol(&attr, -1);
	CU_ASSERT(rc == EINVAL);
}


static void
test_mutexattr_prioceiling(void)
{
	pthread_mutexattr_t attr;
	int rc, val;

	/* Check initialisation */
	rc = pthread_mutexattr_init(&attr);
	CU_ASSERT(rc == 0);

	/* Check prioceiling */
	val = -1;
	rc = wpdk_pthread_mutexattr_getprioceiling(&attr, &val);
	CU_ASSERT(rc == ENOSYS);

	/* Check getprioceiling null attr */
	val = -1;
	rc = wpdk_pthread_mutexattr_getprioceiling(NULL, &val);
	CU_ASSERT(rc == EINVAL);

	/* Check getprioceiling null arg */
	rc = wpdk_pthread_mutexattr_getprioceiling(&attr, NULL);
	CU_ASSERT(rc == EINVAL);

	/* Check setprioceiling */
	val = -1;
	rc = wpdk_pthread_mutexattr_setprioceiling(&attr, 0);
	CU_ASSERT(rc == ENOSYS);

	/* Check setprioceiling null attr */
	rc = wpdk_pthread_mutexattr_setprioceiling(NULL, 0);
	CU_ASSERT(rc == EINVAL);
}


static void
test_mutex_init(void)
{
	pthread_mutexattr_t attr;
	pthread_mutex_t mutex;
	int rc;

	/* Create attributes */
	rc = pthread_mutexattr_init(&attr);
	CU_ASSERT(rc == 0);

	/* Check null mutex */
	rc = pthread_mutex_init(NULL, &attr);
	CU_ASSERT(rc == EINVAL);

	/* Check init */
	rc = pthread_mutex_init(&mutex, &attr);
	CU_ASSERT(rc == 0);
	rc = pthread_mutex_destroy(&mutex);
	CU_ASSERT(rc == 0);

	/* Check null attr */
	memset(&mutex, 0, sizeof(mutex));
	rc = pthread_mutex_init(&mutex, NULL);
	CU_ASSERT(rc == 0);
	rc = pthread_mutex_destroy(&mutex);
	CU_ASSERT(rc == 0);
}


static void
test_mutex_lock(void)
{
	pthread_mutex_t mutex;
	int rc;

	/* Check init */
	rc = pthread_mutex_init(&mutex, NULL);
	CU_ASSERT(rc == 0);

	/* Check lock */
	rc = pthread_mutex_lock(&mutex);
	CU_ASSERT(rc == 0);

	/* Check unlock */
	rc = pthread_mutex_unlock(&mutex);
	CU_ASSERT(rc == 0);

	rc = pthread_mutex_destroy(&mutex);
	CU_ASSERT(rc == 0);
}


static void
test_mutex_trylock(void)
{
	pthread_mutex_t mutex;
	int rc;

	/* Check init */
	rc = pthread_mutex_init(&mutex, NULL);
	CU_ASSERT(rc == 0);

	/* Check trylock */
	rc = pthread_mutex_trylock(&mutex);
	CU_ASSERT(rc == 0);

	/* Check unlock */
	rc = pthread_mutex_unlock(&mutex);
	CU_ASSERT(rc == 0);

	rc = pthread_mutex_destroy(&mutex);
	CU_ASSERT(rc == 0);
}


static void
test_spin_init(void)
{
	pthread_spinlock_t spin;
	int rc;

	/* Check null spinlock */
	rc = pthread_spin_init(NULL, PTHREAD_PROCESS_PRIVATE);
	CU_ASSERT(rc == EINVAL);

	/* Check init */
	rc = pthread_spin_init(&spin, PTHREAD_PROCESS_PRIVATE);
	CU_ASSERT(rc == 0);
	rc = pthread_spin_destroy(&spin);
	CU_ASSERT(rc == 0);
}


static void
test_spin_lock(void)
{
	pthread_spinlock_t spin;
	int rc;

	/* Check init */
	rc = pthread_spin_init(&spin, PTHREAD_PROCESS_PRIVATE);
	CU_ASSERT(rc == 0);

	/* Check lock */
	rc = pthread_spin_lock(&spin);
	CU_ASSERT(rc == 0);

	/* Check unlock */
	rc = pthread_spin_unlock(&spin);
	CU_ASSERT(rc == 0);

	rc = pthread_spin_destroy(&spin);
	CU_ASSERT(rc == 0);
}


static void
test_spin_trylock(void)
{
	pthread_spinlock_t spin;
	int rc;

	/* Check init */
	rc = pthread_spin_init(&spin, PTHREAD_PROCESS_PRIVATE);
	CU_ASSERT(rc == 0);

	/* Check trylock */
	rc = pthread_spin_trylock(&spin);
	CU_ASSERT(rc == 0);

	/* Check unlock */
	rc = pthread_spin_unlock(&spin);
	CU_ASSERT(rc == 0);

	rc = pthread_spin_destroy(&spin);
	CU_ASSERT(rc == 0);
}


static void
test_barrierattr_pshared(void)
{
	pthread_barrierattr_t attr;
	int rc, val;

	/* Check initialisation */
	rc = pthread_barrierattr_init(&attr);
	CU_ASSERT(rc == 0);

	/* Check pshared */
	val = -1;
	rc = wpdk_pthread_barrierattr_getpshared(&attr, &val);
	CU_ASSERT(rc == 0);
	CU_ASSERT(val == PTHREAD_PROCESS_PRIVATE);

	/* Check getpshared null attr */
	val = -1;
	rc = wpdk_pthread_barrierattr_getpshared(NULL, &val);
	CU_ASSERT(rc == EINVAL);

	/* Check getpshared null arg */
	rc = wpdk_pthread_barrierattr_getpshared(&attr, NULL);
	CU_ASSERT(rc == EINVAL);

	/* Check setpshared */
	val = -1;
	rc = wpdk_pthread_barrierattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
	CU_ASSERT(rc == 0);
	rc = wpdk_pthread_barrierattr_getpshared(&attr, &val);
	CU_ASSERT(rc == 0);
	CU_ASSERT(val == PTHREAD_PROCESS_SHARED);

	/* Check setpshared null attr */
	rc = wpdk_pthread_barrierattr_setpshared(NULL, PTHREAD_PROCESS_SHARED);
	CU_ASSERT(rc == EINVAL);

	/* Check invalid pshared */
	rc = wpdk_pthread_barrierattr_setpshared(&attr, -1);
	CU_ASSERT(rc == EINVAL);
}


static void
test_barrier_init(void)
{
	pthread_barrierattr_t attr;
	pthread_barrier_t barrier;
	int rc;

	/* Create attributes */
	rc = pthread_barrierattr_init(&attr);
	CU_ASSERT(rc == 0);

	/* Check null barrier */
	rc = pthread_barrier_init(NULL, &attr, 1);
	CU_ASSERT(rc == EINVAL);

	/* Check zero count */
	rc = pthread_barrier_init(&barrier, &attr, 0);
	CU_ASSERT(rc == EINVAL);

	/* Check init */
	rc = pthread_barrier_init(&barrier, &attr, 1);
	CU_ASSERT(rc == 0);
	rc = pthread_barrier_destroy(&barrier);
	CU_ASSERT(rc == 0);

	/* Check null attr */
	memset(&barrier, 0, sizeof(barrier));
	rc = pthread_barrier_init(&barrier, NULL, 1);
	CU_ASSERT(rc == 0);
	rc = pthread_barrier_destroy(&barrier);
	CU_ASSERT(rc == 0);
}


static void
test_barrier_wait(void)
{
	pthread_barrier_t barrier;
	int rc;

	/* Check init */
	rc = pthread_barrier_init(&barrier, NULL, 1);
	CU_ASSERT(rc == 0);

	rc = pthread_barrier_wait(&barrier);
	CU_ASSERT(rc == PTHREAD_BARRIER_SERIAL_THREAD);

	rc = pthread_barrier_destroy(&barrier);
	CU_ASSERT(rc == 0);
}


static void
test_condattr_pshared(void)
{
	pthread_condattr_t attr;
	int rc, val;

	/* Check initialisation */
	rc = pthread_condattr_init(&attr);
	CU_ASSERT(rc == 0);

	/* Check pshared */
	val = -1;
	rc = wpdk_pthread_condattr_getpshared(&attr, &val);
	CU_ASSERT(rc == 0);
	CU_ASSERT(val == PTHREAD_PROCESS_PRIVATE);

	/* Check getpshared null attr */
	val = -1;
	rc = wpdk_pthread_condattr_getpshared(NULL, &val);
	CU_ASSERT(rc == EINVAL);

	/* Check getpshared null arg */
	rc = wpdk_pthread_condattr_getpshared(&attr, NULL);
	CU_ASSERT(rc == EINVAL);

	/* Check setpshared */
	val = -1;
	rc = wpdk_pthread_condattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
	CU_ASSERT(rc == 0);
	rc = wpdk_pthread_condattr_getpshared(&attr, &val);
	CU_ASSERT(rc == 0);
	CU_ASSERT(val == PTHREAD_PROCESS_SHARED);

	/* Check setpshared null attr */
	rc = wpdk_pthread_condattr_setpshared(NULL, PTHREAD_PROCESS_SHARED);
	CU_ASSERT(rc == EINVAL);

	/* Check invalid pshared */
	rc = wpdk_pthread_condattr_setpshared(&attr, -1);
	CU_ASSERT(rc == EINVAL);
}


static void
test_cond_init(void)
{
	pthread_condattr_t attr;
	pthread_cond_t cond;
	int rc;

	/* Create attributes */
	rc = pthread_condattr_init(&attr);
	CU_ASSERT(rc == 0);

	/* Check null cond */
	rc = pthread_cond_init(NULL, &attr);
	CU_ASSERT(rc == EINVAL);

	/* Check init */
	rc = pthread_cond_init(&cond, &attr);
	CU_ASSERT(rc == 0);
	rc = pthread_cond_destroy(&cond);
	CU_ASSERT(rc == 0);

	/* Check null attr */
	memset(&cond, 0, sizeof(cond));
	rc = pthread_cond_init(&cond, NULL);
	CU_ASSERT(rc == 0);
	rc = pthread_cond_destroy(&cond);
	CU_ASSERT(rc == 0);
}


static void
test_cond_signal(void)
{
	pthread_cond_t cond;
	int rc;

	/* Check init */
	rc = pthread_cond_init(&cond, NULL);
	CU_ASSERT(rc == 0);

	rc = pthread_cond_signal(&cond);
	CU_ASSERT(rc == 0);

	rc = pthread_cond_destroy(&cond);
	CU_ASSERT(rc == 0);
}


static void
test_cond_broadcast(void)
{
	pthread_cond_t cond;
	int rc;

	/* Check init */
	rc = pthread_cond_init(&cond, NULL);
	CU_ASSERT(rc == 0);

	rc = pthread_cond_broadcast(&cond);
	CU_ASSERT(rc == 0);

	rc = pthread_cond_destroy(&cond);
	CU_ASSERT(rc == 0);
}


static void
test_cond_timedwait(void)
{
	struct timespec now, delay;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	int rc;

	/* Check init */
	rc = pthread_mutex_init(&mutex, NULL);
	CU_ASSERT(rc == 0);

	/* Check init */
	rc = pthread_cond_init(&cond, NULL);
	CU_ASSERT(rc == 0);

	/* Check lock */
	rc = pthread_mutex_lock(&mutex);
	CU_ASSERT(rc == 0);

	/* Check timedwait */
	rc = clock_gettime(CLOCK_REALTIME, &now);
	CU_ASSERT(rc == 0);
	rc = pthread_cond_timedwait(&cond, &mutex, &now);
	CU_ASSERT(rc == ETIMEDOUT);

	/* Check 40msec timedwait */
	rc = clock_gettime(CLOCK_REALTIME, &delay);
	CU_ASSERT(rc == 0);
	delay.tv_nsec += 40000000;
	rc = pthread_cond_timedwait(&cond, &mutex, &delay);
	CU_ASSERT(rc == ETIMEDOUT);

	/* Check delay time to within 2 msec*/
	rc = clock_gettime(CLOCK_REALTIME, &now);
	CU_ASSERT(rc == 0);
	CU_ASSERT(now.tv_sec > delay.tv_sec ||
		(now.tv_sec == delay.tv_sec && now.tv_nsec + 2000000 >= delay.tv_nsec));

	rc = pthread_mutex_unlock(&mutex);
	CU_ASSERT(rc == 0);
	rc = pthread_cond_destroy(&cond);
	CU_ASSERT(rc == 0);
	rc = pthread_mutex_destroy(&mutex);
	CU_ASSERT(rc == 0);
}


static void
test_attr_detachstate(void)
{
	pthread_attr_t attr;
	int rc, val;

	/* Check initialisation */
	rc = pthread_attr_init(&attr);
	CU_ASSERT(rc == 0);

	/* Check detachstate */
	val = -1;
	rc = wpdk_pthread_attr_getdetachstate(&attr, &val);
	CU_ASSERT(rc == 0);
	CU_ASSERT(val == PTHREAD_CREATE_JOINABLE);

	/* Check getdetachstate null attr */
	val = -1;
	rc = wpdk_pthread_attr_getdetachstate(NULL, &val);
	CU_ASSERT(rc == EINVAL);

	/* Check getdetachstate null arg */
	rc = wpdk_pthread_attr_getdetachstate(&attr, NULL);
	CU_ASSERT(rc == EINVAL);

	/* Check setdetachstate */
	val = -1;
	rc = wpdk_pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	CU_ASSERT(rc == 0);
	rc = wpdk_pthread_attr_getdetachstate(&attr, &val);
	CU_ASSERT(rc == 0);
	CU_ASSERT(val == PTHREAD_CREATE_DETACHED);

	/* Check setdetachstate null attr */
	rc = wpdk_pthread_attr_setdetachstate(NULL, PTHREAD_CREATE_DETACHED);
	CU_ASSERT(rc == EINVAL);

	/* Check invalid detachstate */
	rc = wpdk_pthread_attr_setdetachstate(&attr, -1);
	CU_ASSERT(rc == EINVAL);
}


static void
test_attr_stacksize(void)
{
	pthread_attr_t attr;
	size_t val;
	int rc;

	/* Check initialisation */
	rc = pthread_attr_init(&attr);
	CU_ASSERT(rc == 0);

	/* Check stacksize */
	val = 0;
	rc = wpdk_pthread_attr_getstacksize(&attr, &val);
	CU_ASSERT(rc == 0);

	/* Check getstacksize null attr */
	val = 0;
	rc = wpdk_pthread_attr_getstacksize(NULL, &val);
	CU_ASSERT(rc == EINVAL);

	/* Check getstacksize null arg */
	rc = wpdk_pthread_attr_getstacksize(&attr, NULL);
	CU_ASSERT(rc == EINVAL);

	/* Check setstacksize */
	val = 0;
	rc = wpdk_pthread_attr_setstacksize(&attr, 1024 * 1024);
	CU_ASSERT(rc == 0);
	rc = wpdk_pthread_attr_getstacksize(&attr, &val);
	CU_ASSERT(rc == 0);
	CU_ASSERT(val == 1024 * 1024);

	/* Check setstacksize null attr */
	rc = wpdk_pthread_attr_setstacksize(NULL, 1024 * 1024);
	CU_ASSERT(rc == EINVAL);
}


static void *
test_create_async(void *arg)
{
	return (void *)(~(ULONG_PTR)arg);
}


static void
test_create(void)
{
	pthread_t thread, threadv[10];
	ULONG_PTR val = 0x5665689;
	pthread_attr_t attr;
	void *result;
	int i, rc;

	/* Check null thread pointer */
	rc = pthread_create(NULL, NULL, test_create_async, (void *)val);
	CU_ASSERT(rc == EINVAL);

	/* Check null start routine */
	rc = pthread_create(&thread, NULL, NULL, (void *)val);
	CU_ASSERT(rc == EINVAL);

	/* Check create */
	thread = 0;
	rc = pthread_create(&thread, NULL, test_create_async, (void *)val);
	CU_ASSERT(rc == 0);
	CU_ASSERT(thread != 0);

	/* Check join */
	result = 0;
	rc = pthread_join(thread, &result);
	CU_ASSERT(rc == 0);
	CU_ASSERT(result == (void *)(~val));

	/* Check create detached */
	rc = pthread_attr_init(&attr);
	CU_ASSERT(rc == 0);
	rc = wpdk_pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	CU_ASSERT(rc == 0);
	thread = 0;
	rc = pthread_create(&thread, &attr, test_create_async, (void *)val);
	CU_ASSERT(rc == 0);
	CU_ASSERT(thread != 0);

	/* Check join fails */
	rc = pthread_join(thread, &result);
	CU_ASSERT(rc == EINVAL);

	/* Check create multiple */
	for (i = 0; i < (int)(sizeof(threadv) / sizeof(threadv[0])); i++) {
		threadv[i] = 0;
		rc = pthread_create(&threadv[i], NULL, test_create_async, (void *)(val+i));
		CU_ASSERT(rc == 0);
		CU_ASSERT(threadv[i] != 0);
	}

	/* Check join multiple */
	for (i = 0; i < (int)(sizeof(threadv) / sizeof(threadv[0])); i++) {
		result = 0;
		rc = pthread_join(threadv[i], &result);
		CU_ASSERT(rc == 0);
		CU_ASSERT(result == (void *)~(val+i));
	}
}


static void
test_join(void)
{
	pthread_t thread, threadv[10];
	ULONG_PTR val = 0x5665689;
	void *result;
	int i, rc;

	/* Check create */
	thread = 0;
	rc = pthread_create(&thread, NULL, test_create_async, (void *)val);
	CU_ASSERT(rc == 0);
	CU_ASSERT(thread != 0);

	/* Check zero thread */
	rc = pthread_join(0, &result);
	CU_ASSERT(rc == EINVAL);

	/* Check join */
	result = 0;
	rc = pthread_join(thread, &result);
	CU_ASSERT(rc == 0);
	CU_ASSERT(result == (void *)(~val));

	/* Check second join */
	result = 0;
	rc = pthread_join(thread, &result);
	CU_ASSERT(rc == EINVAL);

	/* Check create multiple */
	for (i = 0; i < (int)(sizeof(threadv) / sizeof(threadv[0])); i++) {
		threadv[i] = 0;
		rc = pthread_create(&threadv[i], NULL, test_create_async, (void *)(val+i));
		CU_ASSERT(rc == 0);
		CU_ASSERT(threadv[i] != 0);
	}

	/* Check join multiple reverse order */
	for (i = (int)(sizeof(threadv) / sizeof(threadv[0])) - 1; i >= 0; i--) {
		result = 0;
		rc = pthread_join(threadv[i], &result);
		CU_ASSERT(rc == 0);
		CU_ASSERT(result == (void *)~(val+i));
	}
}


static void
test_detach(void)
{
	pthread_t thread;
	void *result;
	int rc;

	/* Check create */
	thread = 0;
	rc = pthread_create(&thread, NULL, test_create_async, NULL);
	CU_ASSERT(rc == 0);
	CU_ASSERT(thread != 0);

	/* Check zero thread */
	rc = pthread_detach(0);
	CU_ASSERT(rc == EINVAL);

	/* Check detach */
	rc = pthread_detach(thread);
	CU_ASSERT(rc == 0);

	/* Check second detach */
	rc = pthread_detach(thread);
	CU_ASSERT(rc == EINVAL);

	/* Check join */
	rc = pthread_join(thread, &result);
	CU_ASSERT(rc == EINVAL);
}


static void *
test_exit_async(void *arg)
{
	pthread_exit((void *)(~(ULONG_PTR)arg));
	return (void *)-1;
}


static void
test_exit(void)
{
	pthread_t thread;
	void *result;
	int rc;

	/* Check create */
	thread = 0;
	rc = pthread_create(&thread, NULL, test_exit_async, (void *)0x1234);
	CU_ASSERT(rc == 0);
	CU_ASSERT(thread != 0);

	/* Check join */
	rc = pthread_join(thread, &result);
	CU_ASSERT(rc == 0);
	CU_ASSERT(result == (void *)~0x1234)
}


static void
test_getaffinity(void)
{
	pthread_t thread;
	cpuset_t set;
	int rc;

	/* Check create */
	thread = 0;
	rc = pthread_create(&thread, NULL, test_exit_async, NULL);
	CU_ASSERT(rc == 0);
	CU_ASSERT(thread != 0);

	/* Check getaffinity */
	rc = pthread_getaffinity_np(thread, sizeof(set), &set);
	CU_ASSERT(rc == 0);

	/* Check join */
	rc = pthread_join(thread, NULL);
	CU_ASSERT(rc == 0);
}


static void
test_setaffinity(void)
{
	pthread_t thread;
	cpuset_t set;
	int rc;

	/* Check create */
	thread = 0;
	rc = pthread_create(&thread, NULL, test_exit_async, NULL);
	CU_ASSERT(rc == 0);
	CU_ASSERT(thread != 0);

	/* Check getaffinity */
	rc = pthread_getaffinity_np(thread, sizeof(set), &set);
	CU_ASSERT(rc == 0);

	/* Check setaffinity */
	rc = wpdk_pthread_setaffinity_np(thread, sizeof(set), &set);
	CU_ASSERT(rc == 0);

	/* Check join */
	rc = pthread_join(thread, NULL);
	CU_ASSERT(rc == 0);
}


static void
test_equal(void)
{
	pthread_t thread, thread2;
	int rc;

	/* Check create */
	thread = 0;
	rc = pthread_create(&thread, NULL, test_create_async, NULL);
	CU_ASSERT(rc == 0);
	CU_ASSERT(thread != 0);

	/* Check second create */
	thread2 = 0;
	rc = pthread_create(&thread2, NULL, test_create_async, NULL);
	CU_ASSERT(rc == 0);
	CU_ASSERT(thread2 != 0);

	/* Check equal */
	rc = (pthread_equal(thread, thread) != 0);
	CU_ASSERT(rc != 0);

	/* Check unequal */
	rc = (pthread_equal(thread, thread2) != 0);
	CU_ASSERT(rc == 0);

	rc = pthread_join(thread, NULL);
	CU_ASSERT(rc == 0);

	rc = pthread_join(thread2, NULL);
	CU_ASSERT(rc == 0);
}


static void
test_key_create(void)
{
	pthread_key_t key;
	int rc;

	/* Check null pointer */
	rc = pthread_key_create(NULL, NULL);
	CU_ASSERT(rc == EINVAL);

	/* Check create */
	rc = pthread_key_create(&key, NULL);
	CU_ASSERT(rc == 0);

	/* Check delete */
	rc = pthread_key_delete(key);
	CU_ASSERT(rc == 0);
}


static void
test_key_delete(void)
{
	pthread_key_t key;
	int rc;

	/* Check create */
	rc = pthread_key_create(&key, NULL);
	CU_ASSERT(rc == 0);

	/* Check delete */
	rc = pthread_key_delete(key);
	CU_ASSERT(rc == 0);

	/* Check second delete */
	rc = pthread_key_delete(key);
	CU_ASSERT(rc == EINVAL);
}


static void
test_getspecific(void)
{
	pthread_key_t key;
	void *val;
	int rc;

	/* Check create */
	rc = pthread_key_create(&key, NULL);
	CU_ASSERT(rc == 0);

	/* Check setspecific */
	rc = pthread_setspecific(key, &rc);
	CU_ASSERT(rc == 0);

	/* Check getspecific */
	val = pthread_getspecific(key);
	CU_ASSERT(val == &rc);

	/* Check delete */
	rc = pthread_key_delete(key);
	CU_ASSERT(rc == 0);

	/* Check getspecific after delete */
	val = pthread_getspecific(key);
	CU_ASSERT(val == NULL);
}


static void *
test_setspecific_async(void *arg)
{
	pthread_key_t key = *(pthread_key_t *)arg;
	void *val;
	int rc;

	/* Check setspecific */
	rc = pthread_setspecific(key, &rc);
	CU_ASSERT(rc == 0);

	/* Check getspecific */
	val = pthread_getspecific(key);
	CU_ASSERT(val == &rc);

	return NULL;
}


static void
test_setspecific(void)
{
	pthread_key_t key;
	pthread_t thread;
	void *val;
	int rc;

	/* Check create */
	rc = pthread_key_create(&key, NULL);
	CU_ASSERT(rc == 0);

	/* Check setspecific */
	rc = pthread_setspecific(key, &rc);
	CU_ASSERT(rc == 0);

	/* Check getspecific */
	val = pthread_getspecific(key);
	CU_ASSERT(val == &rc);

	/* Create thread */
	rc = pthread_create(&thread, NULL, test_setspecific_async, &key);
	CU_ASSERT(rc == 0);

	/* Wait for completion */
	rc = pthread_join(thread, NULL);
	CU_ASSERT(rc == 0);

	/* Check getspecific */
	val = pthread_getspecific(key);
	CU_ASSERT(val == &rc);

	/* Check delete */
	rc = pthread_key_delete(key);
	CU_ASSERT(rc == 0);
}


void
add_pthread_tests()
{
	CU_pSuite suite = NULL;

	suite = CU_add_suite("pthread", null_init, null_clean);

	CU_ADD_TEST(suite, test_mutexattr_type);
	CU_ADD_TEST(suite, test_mutexattr_pshared);
	CU_ADD_TEST(suite, test_mutexattr_robust);
	CU_ADD_TEST(suite, test_mutexattr_protocol);
	CU_ADD_TEST(suite, test_mutexattr_prioceiling);
	CU_ADD_TEST(suite, test_mutex_init);
	CU_ADD_TEST(suite, test_mutex_lock);
	CU_ADD_TEST(suite, test_mutex_trylock);
	CU_ADD_TEST(suite, test_spin_init);
	CU_ADD_TEST(suite, test_spin_lock);
	CU_ADD_TEST(suite, test_spin_trylock);
	CU_ADD_TEST(suite, test_barrierattr_pshared);
	CU_ADD_TEST(suite, test_barrier_init);
	CU_ADD_TEST(suite, test_barrier_wait);
	CU_ADD_TEST(suite, test_condattr_pshared);
	CU_ADD_TEST(suite, test_cond_init);
	CU_ADD_TEST(suite, test_cond_signal);
	CU_ADD_TEST(suite, test_cond_broadcast);
	CU_ADD_TEST(suite, test_cond_timedwait);
	CU_ADD_TEST(suite, test_attr_detachstate);
	CU_ADD_TEST(suite, test_attr_stacksize);
	CU_ADD_TEST(suite, test_create);
	CU_ADD_TEST(suite, test_join);
	CU_ADD_TEST(suite, test_detach);
	CU_ADD_TEST(suite, test_exit);
	CU_ADD_TEST(suite, test_getaffinity);
	CU_ADD_TEST(suite, test_setaffinity);
	CU_ADD_TEST(suite, test_equal);
	CU_ADD_TEST(suite, test_key_create);
	CU_ADD_TEST(suite, test_key_delete);
	CU_ADD_TEST(suite, test_getspecific);
	CU_ADD_TEST(suite, test_setspecific);
}
