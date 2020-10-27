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
#include <sys/stat.h>
#include <termios.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
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
test_tcgetattr(void)
{
	char *path = "testfile";
	struct termios t;
	int fd, rc;

	/* Check tcgetattr */
	rc = tcgetattr(0, &t);
	CU_ASSERT(rc == 0);

	/* Check bad fd */
	rc = tcgetattr(-1, &t);
	CU_ASSERT(rc == -1 && errno == EBADF);

	/* Check null argument */
	rc = tcgetattr(0, NULL);
	CU_ASSERT(rc == -1 && errno == EINVAL);

	unlink(path);

	/* Create file */
	fd = open(path, O_CREAT|O_RDWR, S_IWRITE|S_IREAD);
	CU_ASSERT(fd != -1);

	/* Check file access */
	rc = tcgetattr(fd, &t);
	CU_ASSERT(rc == -1 && errno == ENOTTY);

	close(fd);
	unlink(path);	
}


static void
test_tcsetattr(void)
{
	char *path = "testfile";
	struct termios t;
	int fd, rc;

	memset(&t, 0, sizeof(t));

	/* Check tcsetattr */
	rc = tcsetattr(0, TCSANOW, &t);
	CU_ASSERT(rc == 0);

	/* Check bad fd */
	rc = tcsetattr(-1, TCSANOW, &t);
	CU_ASSERT(rc == -1 && errno == EBADF);

	/* Check null argument */
	rc = tcsetattr(0, TCSANOW, NULL);
	CU_ASSERT(rc == -1 && errno == EINVAL);

	/* Check invalid argument */
	rc = tcsetattr(0, -1, &t);
	CU_ASSERT(rc == -1 && errno == EINVAL);

	unlink(path);

	/* Create file */
	fd = open(path, O_CREAT|O_RDWR, S_IWRITE|S_IREAD);
	CU_ASSERT(fd != -1);

	/* Check file access */
	rc = tcsetattr(fd, TCSANOW, &t);
	CU_ASSERT(rc == -1 && errno == ENOTTY);

	close(fd);
	unlink(path);
}


void add_termios_tests()
{
	CU_pSuite suite = NULL;

	suite = CU_add_suite("termios", null_init, null_clean);

	CU_ADD_TEST(suite, test_tcgetattr);
	CU_ADD_TEST(suite, test_tcsetattr);
}
