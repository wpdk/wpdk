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
#include <signal.h>
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
test_sigset(void)
{
	sigset_t set, full, empty;
	int i, rc;

	/* Check sigemptyset */
	rc = sigemptyset(&empty);
	CU_ASSERT(rc == 0);

	/* Check null sigemptyset */
	rc = sigemptyset(NULL);
	CU_ASSERT(rc == -1 && errno == EINVAL);

	/* Check sigfillset */
	rc = sigfillset(&full);
	CU_ASSERT(rc == 0);

	/* Check null sigfillset */
	rc = sigfillset(NULL);
	CU_ASSERT(rc == -1 && errno == EINVAL);

	/* Check null sigaddset */
	rc = sigaddset(NULL, SIGINT);
	CU_ASSERT(rc == -1 && errno == EINVAL);

	/* Check invalid sigaddset */
	rc = sigaddset(&set, 0);
	CU_ASSERT(rc == -1 && errno == EINVAL);

	/* Check invalid sigaddset */
	rc = sigaddset(&set, 123);
	CU_ASSERT(rc == -1 && errno == EINVAL);

	/* Check all bits */
	rc = sigemptyset(&set);
	CU_ASSERT(rc == 0);
	for (i = 1; i < NSIG; i++) {
		rc = sigaddset(&set, i);
		CU_ASSERT(rc == 0);
	}
	CU_ASSERT(memcmp(&set, &full, sizeof(set)) == 0);

	/* Check null sigdelset */
	rc = sigdelset(NULL, SIGINT);
	CU_ASSERT(rc == -1 && errno == EINVAL);

	/* Check invalid sigdelset */
	rc = sigdelset(&set, 0);
	CU_ASSERT(rc == -1 && errno == EINVAL);

	/* Check invalid sigdelset */
	rc = sigdelset(&set, 123);
	CU_ASSERT(rc == -1 && errno == EINVAL);

	/* Check all bits */
	rc = sigfillset(&set);
	CU_ASSERT(rc == 0);
	for (i = 1; i < NSIG; i++) {
		rc = sigdelset(&set, i);
		CU_ASSERT(rc == 0);
	}
	CU_ASSERT(memcmp(&set, &empty, sizeof(set)) == 0);
}


void add_signal_tests()
{
	CU_pSuite suite = NULL;

	suite = CU_add_suite("signal", null_init, null_clean);

	CU_ADD_TEST(suite, test_sigset);
}
