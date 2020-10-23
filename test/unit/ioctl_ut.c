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
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <net/if.h>
#include <ifaddrs.h>
#include <unistd.h>

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
test_siocgifflags(void)
{
	struct ifaddrs *ifa, *ifaddrs = NULL;
	struct ifreq ifr;
	int rc, fd;

	/* Get list of interfaces */
	rc = getifaddrs(&ifaddrs);
	CU_ASSERT(rc == 0);
	CU_ASSERT(ifaddrs != NULL);

	/* Open socket */
	fd = socket(AF_INET, SOCK_STREAM, 0);
	CU_ASSERT(fd != -1);

	/* Check flags are correct */
	for (ifa = ifaddrs; ifa; ifa = ifa->ifa_next) {
		strcpy(ifr.ifr_name, ifa->ifa_name);
		ifr.ifr_flags = 0;
		rc = ioctl(fd, SIOCGIFFLAGS, &ifr);
		CU_ASSERT(rc == 0);
		CU_ASSERT(ifr.ifr_flags == (short)ifa->ifa_flags);
	}

	freeifaddrs(ifaddrs);
	rc = close(fd);
	CU_ASSERT(rc == 0);
}


void add_ioctl_tests()
{
	CU_pSuite suite = NULL;

	suite = CU_add_suite("ioctl", null_init, null_clean);

	CU_ADD_TEST(suite, test_siocgifflags);
}
