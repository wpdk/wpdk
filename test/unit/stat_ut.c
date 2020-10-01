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
#include <direct.h>
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
test_chmod(void)
{
	char *path = "testfile";
	int fd, rc;

	unlink(path);

	/* Create file */
	fd = open(path, O_CREAT|O_RDWR, S_IWRITE|S_IREAD);
	CU_ASSERT(fd != -1);
	close(fd);

	/* Check file is writable */
	rc = access(path, W_OK);
	CU_ASSERT(rc == 0);

	/* Change mode to read-only */
	rc = chmod(path, S_IRUSR|S_IRGRP|S_IROTH);
	CU_ASSERT(rc == 0);

	/* Check file is read-only */
	rc = access(path, W_OK);
	CU_ASSERT(rc == -1 && errno == EACCES);

	unlink(path);
}


static void
test_mknod(void)
{
	char *path = "testfile";
	int rc;

	unlink(path);

	/* Check invalid argument */
	rc = mknod(NULL, S_IFREG, 0);
	CU_ASSERT(rc == -1 && errno == EINVAL);

	/* Check unsupported type */
	rc = mknod(path, S_IFBLK, 0);
	CU_ASSERT(rc == -1 && errno == ENOSYS);

	/* Check non-zero dev */
	rc = mknod(path, S_IFREG|S_IRWXU|S_IRWXG|S_IRWXO, 10);
	CU_ASSERT(rc == -1 && errno == ENOSYS);

	/* Create file */
	rc = mknod(path, S_IFREG|S_IRWXU|S_IRWXG|S_IRWXO, 0);
	CU_ASSERT(rc == 0);

	/* Check file is writable */
	rc = access(path, W_OK);
	CU_ASSERT(rc == 0);

	/* Try and recreate file */
	rc = mknod(path, S_IFREG|S_IRWXU|S_IRWXG|S_IRWXO, 0);
	CU_ASSERT(rc == -1 && errno == EEXIST);

	unlink(path);

	/* Create file */
	rc = mknod(path, S_IFREG|S_IRUSR|S_IRGRP|S_IROTH, 0);
	CU_ASSERT(rc == 0);

	/* Check file is read-only */
	rc = access(path, W_OK);
	CU_ASSERT(rc == -1 && errno == EACCES);

	unlink(path);
}


static void
test_stat(void)
{
	char *path = "testfile";
	struct stat s;
	int rc;

	unlink(path);

	/* Create file */
	rc = mknod(path, S_IFREG|S_IRWXU|S_IRWXG|S_IRWXO, 0);
	CU_ASSERT(rc == 0);

	/* Check file is writable */
	rc = stat(path, &s);
	CU_ASSERT(rc == 0 && (s.st_mode & S_IWRITE));

	/* Check null path */
	rc = stat(NULL, &s);
	CU_ASSERT(rc == -1 && errno == EINVAL);	

	unlink(path);
}


static void
test_fstat(void)
{
	char *path = "testfile";
	struct stat s;
	int rc, fd;
	
	unlink(path);

	/* Create file */
	fd = open(path, O_CREAT|O_RDWR, S_IWRITE|S_IREAD);
	CU_ASSERT(fd != -1);

	/* Check file is writable */
	rc = fstat(fd, &s);
	CU_ASSERT(rc == 0 && (s.st_mode & S_IWRITE));

	close(fd);

	/* Check bad fd */
	rc = fstat(fd, &s);
	CU_ASSERT(rc == -1 && errno == EBADF);

	unlink(path);
}


static void
test_mkdir(void)
{
	char *dirpath = "testdir";
	int rc;

	_rmdir(dirpath);

	/* Create directory */
	rc = mkdir(dirpath, S_IRWXU);
	CU_ASSERT(rc == 0);

	/* Create existing directory */
	rc = mkdir(dirpath, S_IRWXU);
	CU_ASSERT(rc == -1 && errno == EEXIST);

	_rmdir(dirpath);
}


void add_stat_tests()
{
	CU_pSuite suite = NULL;

	suite = CU_add_suite("stat", null_init, null_clean);

	CU_ADD_TEST(suite, test_chmod);
	CU_ADD_TEST(suite, test_mknod);
	CU_ADD_TEST(suite, test_stat);
	CU_ADD_TEST(suite, test_fstat);
	CU_ADD_TEST(suite, test_mkdir);
}
