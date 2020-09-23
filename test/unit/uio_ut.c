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
#include <sys/uio.h>
#include <unistd.h>
#include <fcntl.h>
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
test_readv(void)
{
	char *path = "testfile";
	struct iovec iov[10];
	char buf[100];
	ssize_t rc;
	int fd;

	unlink(path);

	/* Create file */
	fd = open(path, O_CREAT|O_RDWR, S_IWRITE|S_IREAD);
	CU_ASSERT(fd != -1);

	/* Check write succeeded */
	rc = write(fd, "Hello World\n", 12);
	CU_ASSERT(rc == 12);
	close(fd);

	/* Open file */
	fd = open(path, O_RDWR);
	CU_ASSERT(fd != -1);

	iov[0].iov_base = buf;
	iov[0].iov_len = 3;
	iov[1].iov_base = buf + 3;
	iov[1].iov_len = 5;
	iov[2].iov_base = buf + 8;
	iov[2].iov_len = 4;

	/* Check too many elements */
	rc = readv(fd, iov, IOV_MAX + 10);
	CU_ASSERT(rc == -1 && errno == EINVAL);

	/* Check readv */
	memset(buf, 0, sizeof(buf));
	rc = readv(fd, iov, 3);
	CU_ASSERT(rc == 12);
	CU_ASSERT(strncmp(buf, "Hello World\n", 12) == 0);

	iov[3].iov_base = buf + 12;
	iov[3].iov_len = (size_t)UINT_MAX + 10;

	/* Check element too long */
	rc = readv(fd, iov, 4);
	CU_ASSERT(rc == -1 && errno == EINVAL);

	iov[3].iov_len = SSIZE_MAX;

	/* Check transfer too long */
	rc = readv(fd, iov, 4);
	CU_ASSERT(rc == -1 && errno == EINVAL);
	close(fd);

	/* Check readv to bad fd */
	rc = readv(fd, iov, 3);
	CU_ASSERT(rc == -1 && errno == EBADF);

	unlink(path);
}


static void
test_writev(void)
{
	char *path = "testfile";
	struct iovec iov[10];
	char buf[100];
	ssize_t rc;
	int fd;

	unlink(path);

	/* Create file */
	fd = open(path, O_CREAT|O_RDWR, S_IWRITE|S_IREAD);
	CU_ASSERT(fd != -1);

	iov[0].iov_base = buf;
	iov[0].iov_len = 3;
	iov[1].iov_base = buf + 3;
	iov[1].iov_len = 5;
	iov[2].iov_base = buf + 8;
	iov[2].iov_len = 4;

	/* Check writev */
	strcpy(buf, "Hello World\n");
	rc = writev(fd, iov, 3);
	CU_ASSERT(rc == 12);
	CU_ASSERT(strncmp(buf, "Hello World\n", 12) == 0);

	/* Check too many elements */
	rc = writev(fd, iov, IOV_MAX + 10);
	CU_ASSERT(rc == -1 && errno == EINVAL);

	iov[3].iov_base = buf + 12;
	iov[3].iov_len = (size_t)UINT_MAX + 10;

	/* Check element too long */
	rc = writev(fd, iov, 4);
	CU_ASSERT(rc == -1 && errno == EINVAL);

	iov[3].iov_len = SSIZE_MAX;

	/* Check transfer too long */
	rc = writev(fd, iov, 4);
	CU_ASSERT(rc == -1 && errno == EINVAL);
	close(fd);

	/* Check writev to bad fd */
	rc = writev(fd, iov, 3);
	CU_ASSERT(rc == -1 && errno == EBADF);

	/* Open file */
	fd = open(path, O_RDWR);
	CU_ASSERT(fd != -1);

	/* Check data */
	memset(buf, 0, sizeof(buf));
	rc = read(fd, buf, 12);
	CU_ASSERT(rc == 12);
	CU_ASSERT(strncmp(buf, "Hello World\n", 12) == 0);
	close(fd);

	unlink(path);
}


void add_uio_tests()
{
	CU_pSuite suite = NULL;

	suite = CU_add_suite("uio", null_init, null_clean);

	CU_ADD_TEST(suite, test_readv);
	CU_ADD_TEST(suite, test_writev);
}
