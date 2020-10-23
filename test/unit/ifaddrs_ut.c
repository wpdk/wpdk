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
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ifaddrs.h>

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
test_getifaddrs(void)
{
	struct ifaddrs *ifaddr = NULL;
	int rc;

	rc = getifaddrs(&ifaddr);
	CU_ASSERT(rc == 0);
	CU_ASSERT(ifaddr != NULL);

	freeifaddrs(ifaddr);
}


void add_ifaddrs_tests()
{
	CU_pSuite suite = NULL;

	suite = CU_add_suite("ifaddrs", null_init, null_clean);

	CU_ADD_TEST(suite, test_getifaddrs);
}
