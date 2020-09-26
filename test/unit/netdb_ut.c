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
#include <netdb.h>
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
test_getaddrinfo(void)
{
    struct addrinfo *ap, *info = NULL;
    struct addrinfo hints;
	DWORD rc;
	int i;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;	

	/* Check getaddrinfo */
    rc = getaddrinfo(NULL, "echo", &hints, &info);
	CU_ASSERT(rc == 0);

	/* Check info list */
	for (ap = info, i = 0; ap != NULL; i++, ap = ap->ai_next)
		CU_ASSERT(ap->ai_protocol == IPPROTO_TCP);

	/* Check number of entries */
	CU_ASSERT(i != 0);

	freeaddrinfo(info);
}


static void
test_freeaddrinfo(void)
{
    struct addrinfo *info = NULL;
    struct addrinfo hints;
	DWORD rc;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;	

	/* Check getaddrinfo */
    rc = getaddrinfo(NULL, "echo", &hints, &info);
	CU_ASSERT(rc == 0);

	/* Check freeaddrinfo */
	freeaddrinfo(info);

	/* Check null info */
	freeaddrinfo(NULL);
}


static void
test_gai_strerror(void)
{
	const char *msg;

	/* Check msg is returned */
	msg = gai_strerror(EAI_AGAIN);
	CU_ASSERT(*msg != 0);

	/* Check success */
	msg = gai_strerror(0);
	CU_ASSERT(*msg != 0);

	/* Check invalid msg */
	msg = gai_strerror(19099);
	CU_ASSERT(*msg != 0);
}


void add_netdb_tests()
{
	CU_pSuite suite = NULL;

	suite = CU_add_suite("netdb", null_init, null_clean);

	CU_ADD_TEST(suite, test_getaddrinfo);
	CU_ADD_TEST(suite, test_freeaddrinfo);
	CU_ADD_TEST(suite, test_gai_strerror);
}
