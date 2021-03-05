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
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

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
test_flock(void)
{
	char path[] = "/tmp/testXXXXXX";
	int rc, fd1, fd2;
	ssize_t nbytes;

	/* Create test file */
	fd1 = mkstemp(path);
	CU_ASSERT(fd1 != -1);

	/* Add some data */
	nbytes = write(fd1, "Hello World!", 12);
	CU_ASSERT(nbytes == 12);

	/* Open second fd */
	fd2 = open(path, O_RDWR);
	CU_ASSERT(fd2 != -1);

	/* Shared lock */
	rc = flock(fd1, LOCK_SH);
	CU_ASSERT(rc == 0);

	/* Second shared lock */
	rc = flock(fd2, LOCK_SH);
	CU_ASSERT(rc == 0);

	/* Convert to exclusive lock */
	rc = flock(fd2, LOCK_EX | LOCK_NB);
	CU_ASSERT(rc == -1 && errno == EWOULDBLOCK);

	/* Exclusive lock with shared */
	rc = flock(fd2, LOCK_EX | LOCK_NB);
	CU_ASSERT(rc == -1 && errno == EWOULDBLOCK);

	/* Convert to exclusive lock */
	rc = flock(fd1, LOCK_EX);
	CU_ASSERT(rc == 0);

	/* Unlock exclusive lock */
	rc = flock(fd1, LOCK_EX);
	CU_ASSERT(rc == 0);

	/* Unlock without lock */
	rc = flock(fd1, LOCK_EX);
	CU_ASSERT(rc == 0);

	close(fd1);
	close(fd2);
	unlink(path);
}


void add_file_tests()
{
	CU_pSuite suite = NULL;

	suite = CU_add_suite("file", null_init, null_clean);

	CU_ADD_TEST(suite, test_flock);
}
