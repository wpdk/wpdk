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
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
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
test_fcntl_getfd(void)
{
	char *path = "testfile";
	int rc, flags, fd;
	
	unlink(path);

	/* Create test file */
	fd = open(path, O_CREAT|O_RDWR, S_IWRITE|S_IREAD);
	CU_ASSERT(fd != -1);

	flags = fcntl(fd, F_GETFD);
	CU_ASSERT(flags == 0);

	rc = fcntl(fd, F_SETFD, 0);
	CU_ASSERT(rc == 0);

	close(fd);
	unlink(path);
}


static void
test_fcntl_lock(void)
{
	char path[] = "/tmp/testXXXXXX";
	int rc, fd1, fd2;
	struct flock fl;
	ssize_t nbytes;
	off_t posn;

	/* Create test file */
	fd1 = mkstemp(path);
	CU_ASSERT(fd1 != -1);

	/* Add some data */
	nbytes = write(fd1, "Hello World!", 12);
	CU_ASSERT(nbytes == 12);

	/* Open second fd */
	fd2 = open(path, O_RDWR);
	CU_ASSERT(fd2 != -1);

	/* Set to offset 0 */
	posn = lseek(fd1, 0, SEEK_SET);
	CU_ASSERT(posn == 0);

	/* Lock 10 bytes */
	fl.l_type = F_WRLCK;
	fl.l_whence = SEEK_CUR;
	fl.l_start = 0;
	fl.l_len = 10;
	fl.l_pid = 0;
	rc = fcntl(fd1, F_SETLK, &fl);
	CU_ASSERT(rc == 0);

	/* Check locked */
	fl.l_type = F_WRLCK;
	fl.l_whence = SEEK_CUR;
	fl.l_start = 0;
	fl.l_len = 10;
	fl.l_pid = 0;
	rc = fcntl(fd1, F_GETLK, &fl);
	CU_ASSERT(rc == 0);
	CU_ASSERT(fl.l_type != F_UNLCK);

	/* Check offset 10 is unlocked */
	fl.l_type = F_WRLCK;
	fl.l_whence = SEEK_SET;
	fl.l_start = 10;
	fl.l_len = 10;
	fl.l_pid = 0;
	rc = fcntl(fd1, F_GETLK, &fl);
	CU_ASSERT(rc == 0);
	CU_ASSERT(fl.l_type == F_UNLCK);

	/* Lock to end of file */
	fl.l_type = F_WRLCK;
	fl.l_whence = SEEK_SET;
	fl.l_start = 10;
	fl.l_len = 0;
	fl.l_pid = 0;
	rc = fcntl(fd1, F_SETLKW, &fl);
	CU_ASSERT(rc == 0);

	/* Check locked */
	fl.l_type = F_WRLCK;
	fl.l_whence = SEEK_SET;
	fl.l_start = 10;
	fl.l_len = 0;
	fl.l_pid = 0;
	rc = fcntl(fd1, F_GETLK, &fl);
	CU_ASSERT(rc == 0);
	CU_ASSERT(fl.l_type != F_UNLCK);

	/* Check lock fails */
	fl.l_type = F_WRLCK;
	fl.l_whence = SEEK_SET;
	fl.l_start = 10;
	fl.l_len = 0;
	fl.l_pid = 0;
	rc = fcntl(fd2, F_SETLK, &fl);
	CU_ASSERT(rc == -1 && (errno == EACCES || errno == EAGAIN));

	/* Unlock first 10 bytes */
	fl.l_type = F_UNLCK;
	fl.l_whence = SEEK_SET;
	fl.l_start = 0;
	fl.l_len = 10;
	fl.l_pid = 0;
	rc = fcntl(fd1, F_SETLK, &fl);
	CU_ASSERT(rc == 0);

	/* Unlock to end */
	fl.l_type = F_UNLCK;
	fl.l_whence = SEEK_SET;
	fl.l_start = 10;
	fl.l_len = 0;
	fl.l_pid = 0;
	rc = fcntl(fd1, F_SETLK, &fl);
	CU_ASSERT(rc == 0);

	/* Check unlocked to end */
	fl.l_type = F_WRLCK;
	fl.l_whence = SEEK_SET;
	fl.l_start = 0;
	fl.l_len = 0;
	fl.l_pid = 0;
	rc = fcntl(fd1, F_GETLK, &fl);
	CU_ASSERT(rc == 0);
	CU_ASSERT(fl.l_type == F_UNLCK);

	close(fd1);
	close(fd2);
	unlink(path);
}


static void
test_lockfile_get_range(void)
{
	char path[] = "/tmp/testXXXXXX";
	off_t start, nbytes;
	ssize_t posn;
	int rc, fd;

	/* Create test file */
	fd = mkstemp(path);
	CU_ASSERT(fd != -1);

	/* Add some data */
	nbytes = write(fd, "Hello World!", 12);
	CU_ASSERT(nbytes == 12);

	/* Set to offset 5 */
	posn = lseek(fd, 5, SEEK_SET);
	CU_ASSERT(posn == 5);

	/* Check set offset */
	rc = wpdk_lockfile_get_range(fd, SEEK_SET, 100, 192, &start, &nbytes);
	CU_ASSERT(rc == 0);
	CU_ASSERT(start == 100);
	CU_ASSERT(nbytes == 192);

	/* Check current offset */
	rc = wpdk_lockfile_get_range(fd, SEEK_CUR, 0, 501, &start, &nbytes);
	CU_ASSERT(rc == 0);
	CU_ASSERT(start == 5);
	CU_ASSERT(nbytes == 501);

	/* Check end offset */
	rc = wpdk_lockfile_get_range(fd, SEEK_END, 0, 1, &start, &nbytes);
	CU_ASSERT(rc == 0);
	CU_ASSERT(start == 12);
	CU_ASSERT(nbytes == 1);

	/* Check positive offset */
	rc = wpdk_lockfile_get_range(fd, SEEK_CUR, 100, 9, &start, &nbytes);
	CU_ASSERT(rc == 0);
	CU_ASSERT(start == 105);
	CU_ASSERT(nbytes == 9);

	/* Check negative offset */
	rc = wpdk_lockfile_get_range(fd, SEEK_CUR, -3, 19, &start, &nbytes);
	CU_ASSERT(rc == 0);
	CU_ASSERT(start == 2);
	CU_ASSERT(nbytes == 19);

	/* Check negative size */
	rc = wpdk_lockfile_get_range(fd, SEEK_CUR, 0, -2, &start, &nbytes);
	CU_ASSERT(rc == 0);
	CU_ASSERT(start == 3);
	CU_ASSERT(nbytes == 2);

	/* Check zero size */
	rc = wpdk_lockfile_get_range(fd, SEEK_CUR, 0, 0, &start, &nbytes);
	CU_ASSERT(rc == 0);
	CU_ASSERT(start == 5);
	CU_ASSERT(nbytes == LOCKFILE_MAX - 5);

	/* Check negative size too large */
	rc = wpdk_lockfile_get_range(fd, SEEK_CUR, 0, -20, &start, &nbytes);
	CU_ASSERT(rc == -1 && errno == EINVAL);

	/* Check offset too large */
	rc = wpdk_lockfile_get_range(fd, SEEK_SET, INT64_MAX, 33, &start, &nbytes);
	CU_ASSERT(rc == -1 && errno == EINVAL);

	/* Check negative too large offset */
	rc = wpdk_lockfile_get_range(fd, SEEK_CUR, -100, 19, &start, &nbytes);
	CU_ASSERT(rc == -1 && errno == EINVAL);

	/* Check positive too large offset */
	rc = wpdk_lockfile_get_range(fd, SEEK_CUR, INT64_MAX, 3, &start, &nbytes);
	CU_ASSERT(rc == -1 && errno == EINVAL);
	
	/* Check large current position */
	posn = lseek(fd, LOCKFILE_MAX - 10, SEEK_SET);
	CU_ASSERT(posn == LOCKFILE_MAX - 10);
	rc = wpdk_lockfile_get_range(fd, SEEK_CUR, 0, 1, &start, &nbytes);
	CU_ASSERT(rc == 0);
	CU_ASSERT(start == LOCKFILE_MAX - 10);
	CU_ASSERT(nbytes == 1);
	
	/* Check large current position with size too large */
	posn = lseek(fd, LOCKFILE_MAX - 10, SEEK_SET);
	CU_ASSERT(posn == LOCKFILE_MAX - 10);
	rc = wpdk_lockfile_get_range(fd, SEEK_CUR, 0, 12, &start, &nbytes);
	CU_ASSERT(rc == -1 && errno == EINVAL);
	
	/* Check large current position with large negative offset */
	posn = lseek(fd, LOCKFILE_MAX - 10, SEEK_SET);
	CU_ASSERT(posn == LOCKFILE_MAX - 10);
	rc = wpdk_lockfile_get_range(fd, SEEK_CUR, -(LOCKFILE_MAX - 11), 1, &start, &nbytes);
	CU_ASSERT(rc == 0);
	CU_ASSERT(start == 1);
	CU_ASSERT(nbytes == 1);
	
	/* Check large current position with large negative size */
	posn = lseek(fd, LOCKFILE_MAX - 10, SEEK_SET);
	CU_ASSERT(posn == LOCKFILE_MAX - 10);
	rc = wpdk_lockfile_get_range(fd, SEEK_CUR, 0, -(LOCKFILE_MAX - 11), &start, &nbytes);
	CU_ASSERT(rc == 0);
	CU_ASSERT(start == 1);
	CU_ASSERT(nbytes == LOCKFILE_MAX - 11);

	/* Check current position too large */
	posn = lseek(fd, LOCKFILE_MAX, SEEK_SET);
	CU_ASSERT(posn == LOCKFILE_MAX);
	rc = wpdk_lockfile_get_range(fd, SEEK_CUR, 0, 1, &start, &nbytes);
	CU_ASSERT(rc == -1 && errno == EINVAL);

	/* Check invalid file */
	posn = lseek(fd, 5, SEEK_SET);
	CU_ASSERT(posn == 5);
	rc = wpdk_lockfile_get_range(-1, SEEK_CUR, 0, 1, &start, &nbytes);
	CU_ASSERT(rc == -1 && errno == EBADF);

	close(fd);
	unlink(path);
}


void add_fcntl_tests()
{
	CU_pSuite suite = NULL;

	suite = CU_add_suite("fcntl", null_init, null_clean);

	CU_ADD_TEST(suite, test_fcntl_lock);
	CU_ADD_TEST(suite, test_fcntl_getfd);
	CU_ADD_TEST(suite, test_lockfile_get_range);
}
