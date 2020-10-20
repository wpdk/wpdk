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
test_mutexattr(void)
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

	/* Check invalid pshared */
	rc = wpdk_pthread_mutexattr_setrobust(&attr, -1);
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

	/* Check setprioceiling */
	val = -1;
	rc = wpdk_pthread_mutexattr_setprioceiling(&attr, 0);
	CU_ASSERT(rc == ENOSYS);

	/* Check setprotocol null attr */
	rc = wpdk_pthread_mutexattr_setprioceiling(NULL, 0);
	CU_ASSERT(rc == EINVAL);
}


void add_pthread_tests()
{
	CU_pSuite suite = NULL;

	suite = CU_add_suite("pthread", null_init, null_clean);

	CU_ADD_TEST(suite, test_mutexattr);
}
