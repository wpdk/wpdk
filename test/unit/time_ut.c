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
test_clock_gettime(void)
{
	struct timespec t;
	int rc;

	/* Check null timespec */
	rc = clock_gettime(CLOCK_REALTIME, NULL);
	CU_ASSERT(rc == -1 && errno == EINVAL);

	/* Check invalid clock */
	rc = clock_gettime(-1, &t);
	CU_ASSERT(rc == -1 && errno == EINVAL);

	/* Check realtime */
	rc = clock_gettime(CLOCK_REALTIME, &t);
	CU_ASSERT(rc == 0);
	CU_ASSERT(t.tv_nsec < 1000000000);
	CU_ASSERT(t.tv_sec > 50LL * 365 * 24 * 60 * 60);

	/* Check monotonic */
	rc = clock_gettime(CLOCK_MONOTONIC, &t);
	CU_ASSERT(rc == 0);
	CU_ASSERT(t.tv_sec > 0 && t.tv_nsec < 1000000000);

	/* Check monotonic raw */
	rc = clock_gettime(CLOCK_MONOTONIC_RAW, &t);
	CU_ASSERT(rc == 0);
	CU_ASSERT(t.tv_sec > 0 && t.tv_nsec < 1000000000);
}


static void
test_gettimeofday(void)
{
	struct timeval t;
	int rc;

	/* Check gettimeofday */
	rc = gettimeofday(&t, NULL);
	CU_ASSERT(rc == 0);
	CU_ASSERT(t.tv_usec < 1000000);
	CU_ASSERT(t.tv_sec > 50LL * 365 * 24 * 60 * 60);

	/* Check timezone provided */
	rc = gettimeofday(&t, &t);
	CU_ASSERT(rc == -1 && errno == EINVAL);
}


void add_time_tests()
{
	CU_pSuite suite = NULL;

	suite = CU_add_suite("time", null_init, null_clean);

	CU_ADD_TEST(suite, test_clock_gettime);
	CU_ADD_TEST(suite, test_gettimeofday);
}
