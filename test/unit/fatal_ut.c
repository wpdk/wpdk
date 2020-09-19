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
#include <stdio.h>
#include <errno.h>

#include <CUnit/Basic.h>


void expect_fatal(const char *function);
int check_fatal(void);


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
check_unimplemented(void)
{
	expect_fatal(__func__);
	CU_ASSERT(check_fatal() == 0);

	expect_fatal(__func__);
	WPDK_UNIMPLEMENTED();
	CU_ASSERT(check_fatal() == 1);
	CU_ASSERT(check_fatal() == 0);
}


void
add_fatal_tests()
{
	CU_pSuite suite = NULL;

	suite = CU_add_suite("fatal", null_init, null_clean);

	CU_ADD_TEST(suite, check_unimplemented);
}
