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
}
