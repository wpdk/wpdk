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
#include <sys/resource.h>
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
test_getrlimit(void)
{
	struct rlimit lim;
	int rc;

	memset(&lim, 0xff, sizeof(lim));

	/* Check core */
	rc = getrlimit(RLIMIT_CORE, &lim);
	CU_ASSERT(rc == 0);
	CU_ASSERT(lim.rlim_cur == 0);
	CU_ASSERT(lim.rlim_max == 0);

	/* Check nofile */
	rc = getrlimit(RLIMIT_NOFILE, &lim);
	CU_ASSERT(rc == 0);
	CU_ASSERT(lim.rlim_cur > 100);
	CU_ASSERT(lim.rlim_max > 100);

	/* Check invalid type */
	lim.rlim_cur = 100000;
	lim.rlim_max = 100;
	rc = getrlimit(-1, &lim);
	CU_ASSERT(rc == -1 && errno == EINVAL);
}


static void
test_setrlimit(void)
{
	struct rlimit lim;
	int rc;

	/* Set core */
	lim.rlim_cur = 0;
	lim.rlim_max = 0;
	rc = setrlimit(RLIMIT_CORE, &lim);
	CU_ASSERT(rc == 0);

	/* Set nofile */
	lim.rlim_cur = 100;
	lim.rlim_max = 100;
	rc = setrlimit(RLIMIT_NOFILE, &lim);
	CU_ASSERT(rc == 0);

	/* Check nofile > max */
	lim.rlim_cur = 100000;
	lim.rlim_max = 100000;
	rc = setrlimit(RLIMIT_NOFILE, &lim);
	CU_ASSERT(rc == -1 && errno == EPERM);

	/* Check current > max */
	lim.rlim_cur = 100000;
	lim.rlim_max = 100;
	rc = setrlimit(RLIMIT_NOFILE, &lim);
	CU_ASSERT(rc == -1 && errno == EINVAL);

	/* Check invalid type */
	lim.rlim_cur = 100000;
	lim.rlim_max = 100;
	rc = setrlimit(-1, &lim);
	CU_ASSERT(rc == -1 && errno == EINVAL);
}


static void
test_getrusage(void)
{
	struct rusage u;
	int rc;

	/* Get current usage */
	rc = getrusage(RUSAGE_THREAD, &u);
	CU_ASSERT(rc == 0);

	/* Check that some time has been recorded */
	CU_ASSERT(u.ru_utime.tv_sec != 0
		|| u.ru_utime.tv_usec != 0
		|| u.ru_stime.tv_sec != 0
		|| u.ru_stime.tv_usec != 0);
}


void add_resource_tests()
{
	CU_pSuite suite = NULL;

	suite = CU_add_suite("resource", null_init, null_clean);

	CU_ADD_TEST(suite, test_getrlimit);
	CU_ADD_TEST(suite, test_setrlimit);
	CU_ADD_TEST(suite, test_getrusage);
}
