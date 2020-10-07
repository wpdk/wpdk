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
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>

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
test_sem_init(void)
{
	sem_t sem;
	int rc;

	/* Check shared init */
	rc = sem_init(&sem, 1, 10);
	CU_ASSERT(rc == -1 && errno == ENOSYS);

	/* Check value too big */
	rc = sem_init(&sem, 1, UINT_MAX);
	CU_ASSERT(rc == -1 && errno == EINVAL);

	/* Check init */
	rc = sem_init(&sem, 0, 10);
	CU_ASSERT(rc == 0);

	/* Check destroy */
	rc = sem_destroy(&sem);
	CU_ASSERT(rc == 0);
}


static void
test_sem_destroy(void)
{
	sem_t sem;
	int rc;

	/* Check init */
	rc = sem_init(&sem, 0, 10);
	CU_ASSERT(rc == 0);

	/* Check destroy */
	rc = sem_destroy(&sem);
	CU_ASSERT(rc == 0);

	/* Check second destroy */
	rc = sem_destroy(&sem);
	CU_ASSERT(rc == -1 && errno == EINVAL);
}


static void
test_sem_trywait(void)
{
	sem_t sem;
	int rc;

	/* Check init */
	rc = sem_init(&sem, 0, 1);
	CU_ASSERT(rc == 0);

	/* Check trywait */
	rc = sem_trywait(&sem);
	CU_ASSERT(rc == 0);

	/* Check second trywait */
	rc = sem_trywait(&sem);
	CU_ASSERT(rc == -1 && errno == EAGAIN);

	/* Check destroy */
	rc = sem_destroy(&sem);
	CU_ASSERT(rc == 0);
}


static void
test_sem_wait(void)
{
	sem_t sem;
	int rc;

	/* Check init */
	rc = sem_init(&sem, 0, 1);
	CU_ASSERT(rc == 0);

	/* Check wait */
	rc = sem_wait(&sem);
	CU_ASSERT(rc == 0);

	/* Check trywait */
	rc = sem_trywait(&sem);
	CU_ASSERT(rc == -1 && errno == EAGAIN);

	/* Check destroy */
	rc = sem_destroy(&sem);
	CU_ASSERT(rc == 0);
}


static void
test_sem_timedwait(void)
{
	struct timespec now, delay;
	sem_t sem;
	int rc;

	/* Check init */
	rc = sem_init(&sem, 0, 1);
	CU_ASSERT(rc == 0);

	/* Check timedwait */
	rc = clock_gettime(CLOCK_REALTIME, &now);
	CU_ASSERT(rc == 0);
	rc = sem_timedwait(&sem, &now);
	CU_ASSERT(rc == 0);

	/* Check second timedwait */
	rc = sem_timedwait(&sem, &now);
	CU_ASSERT(rc == -1 && errno == EAGAIN);

	/* Check 40msec timedwait */
	rc = clock_gettime(CLOCK_REALTIME, &delay);
	CU_ASSERT(rc == 0);
	delay.tv_nsec += 40000000;
	rc = sem_timedwait(&sem, &delay);
	CU_ASSERT(rc == -1 && errno == EAGAIN);

	/* Check delay time */
	rc = clock_gettime(CLOCK_REALTIME, &now);
	CU_ASSERT(rc == 0);
	CU_ASSERT(now.tv_sec > delay.tv_sec ||
		(now.tv_sec == delay.tv_sec && now.tv_nsec >= delay.tv_nsec));

	/* Check trywait */
	rc = sem_trywait(&sem);
	CU_ASSERT(rc == -1 && errno == EAGAIN);

	/* Check destroy */
	rc = sem_destroy(&sem);
	CU_ASSERT(rc == 0);
}


static void
test_sem_post(void)
{
	sem_t sem;
	int rc;

	/* Check init */
	rc = sem_init(&sem, 0, 1);
	CU_ASSERT(rc == 0);

	/* Check wait */
	rc = sem_wait(&sem);
	CU_ASSERT(rc == 0);

	/* Check post */
	rc = sem_post(&sem);
	CU_ASSERT(rc == 0);

	/* Check second wait */
	rc = sem_wait(&sem);
	CU_ASSERT(rc == 0);

	/* Check trywait */
	rc = sem_trywait(&sem);
	CU_ASSERT(rc == -1 && errno == EAGAIN);

	/* Check destroy */
	rc = sem_destroy(&sem);
	CU_ASSERT(rc == 0);
}


void add_semaphore_tests()
{
	CU_pSuite suite = NULL;

	suite = CU_add_suite("semaphore", null_init, null_clean);

	CU_ADD_TEST(suite, test_sem_init);
	CU_ADD_TEST(suite, test_sem_destroy);
	CU_ADD_TEST(suite, test_sem_trywait);
	CU_ADD_TEST(suite, test_sem_wait);
	CU_ADD_TEST(suite, test_sem_timedwait);
	CU_ADD_TEST(suite, test_sem_post);
}
