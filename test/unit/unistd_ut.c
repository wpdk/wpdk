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
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
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
test_access(void)
{
	char *path = "testfile";
	int fd, rc;

	unlink(path);
	
	/* Check non-existent file */
	rc = access(path, F_OK);
	CU_ASSERT(rc == -1 && errno == ENOENT);

	/* Create file */
	fd = open(path, O_CREAT|O_RDWR, S_IWRITE|S_IREAD);
	CU_ASSERT(fd != -1);
	close(fd);

	/* Check newly created file */
	rc = access(path, R_OK|W_OK);
	CU_ASSERT(rc == 0);

	unlink(path);
}


static void
test_read(void)
{
	char *path = "testfile";
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

	/* Check partial read */
	rc = read(fd, buf, 5);
	CU_ASSERT(rc == 5);
	CU_ASSERT(strncmp(buf, "Hello", 5) == 0);

	/* Check next read */
	rc = read(fd, buf + 5, 5);
	CU_ASSERT(rc == 5);
	CU_ASSERT(strncmp(buf, "Hello Worl", 10) == 0);

	/* Check final read */
	rc = read(fd, buf + 10, 5);
	CU_ASSERT(rc == 2);
	CU_ASSERT(strncmp(buf, "Hello World\n", 12) == 0);

	/* Check read to bad fd */
	close(fd);
	rc = read(fd, buf, 12);
	CU_ASSERT(rc == -1 && errno == EBADF);

	unlink(path);
}


static void
test_write(void)
{
	char *path = "testfile";
	char buf[100];
	ssize_t rc;
	int fd;

	unlink(path);

	/* Create file */
	fd = open(path, O_CREAT|O_RDWR, S_IWRITE|S_IREAD);
	CU_ASSERT(fd != -1);

	/* Check initial write */
	rc = write(fd, "Hello", 5);
	CU_ASSERT(rc == 5);

	/* Check next write */
	rc = write(fd, " Worl", 5);
	CU_ASSERT(rc == 5);

	/* Check final write */
	rc = write(fd, "d\n", 2);
	CU_ASSERT(rc == 2);

	/* Check write to bad fd */
	close(fd);
	memset(buf, 0, sizeof(buf));
	rc = write(fd, buf, 12);
	CU_ASSERT(rc == -1 && errno == EBADF);

	/* Open file */
	fd = open(path, O_RDWR);
	CU_ASSERT(fd != -1);

	/* Check write succeeded */
	memset(buf, 0, sizeof(buf));
	rc = read(fd, buf, 12);
	CU_ASSERT(rc == 12);
	CU_ASSERT(strncmp(buf, "Hello World\n", 12) == 0)
	close(fd);

	unlink(path);
}


static void
test_lseek(void)
{
	char *path = "testfile";
	char buf[100];
	ssize_t rc;
	off_t posn;
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

	/* Check seek */
	posn = lseek(fd, 3, SEEK_SET);
	CU_ASSERT(posn == 3);

	/* Check partial read */
	rc = read(fd, buf, 5);
	CU_ASSERT(rc == 5);
	CU_ASSERT(strncmp(buf, "lo Wo", 5) == 0);

	/* Check current position */
	posn = lseek(fd, 0, SEEK_CUR);
	CU_ASSERT(posn == 8);

	/* Check lseek to bad fd */
	close(fd);
	posn = lseek(fd, 0, SEEK_SET);
	CU_ASSERT(posn == -1 && errno == EBADF);

	unlink(path);
}


static void
test_unlink(void)
{
	char *path = "testfile";
	int fd, rc;

	unlink(path);

	/* Check unlink null pathname */
	rc = unlink("");
	CU_ASSERT(rc == -1 && errno == ENOENT);

	/* Check non-existent file */
	rc = access(path, F_OK);
	CU_ASSERT(rc == -1 && errno == ENOENT);

	/* Check unlink non-existant file */
	rc = unlink(path);
	CU_ASSERT(rc == -1 && errno == ENOENT);

	/* Create file */
	fd = open(path, O_CREAT|O_RDWR, S_IWRITE|S_IREAD);
	CU_ASSERT(fd != -1);
	close(fd);

	/* Check newly created file */
	rc = access(path, R_OK|W_OK);
	CU_ASSERT(rc == 0);

	/* Check unlink file */
	rc = unlink(path);
	CU_ASSERT(rc == 0);

	/* Create read-only file */
	fd = open(path, O_CREAT|O_RDONLY, S_IREAD);
	CU_ASSERT(fd != -1);
	close(fd);

	/* Check unlink read-only file */
	rc = unlink(path);
	CU_ASSERT(rc == 0);
}


static void
test_fsync(void)
{
	char *path = "testfile";
	ssize_t len;
	int fd, rc;

	unlink(path);

	/* Create file */
	fd = open(path, O_CREAT|O_RDWR, S_IWRITE|S_IREAD);
	CU_ASSERT(fd != -1);

	/* Check write succeeded */
	len = write(fd, "Hello World\n", 12);
	CU_ASSERT(len == 12);

	/* Check fsync succeeds */
	rc = fsync(fd);
	CU_ASSERT(rc == 0);
	close(fd);

	/* Check fsync on bad fd */
	rc = fsync(fd);
	CU_ASSERT(rc == -1 && errno == EBADF);

	unlink(path);
}


static void
test_close(void)
{
	char *path = "testfile";
	ssize_t len;
	int fd, rc;

	unlink(path);

	/* Create file */
	fd = open(path, O_CREAT|O_RDWR, S_IWRITE|S_IREAD);
	CU_ASSERT(fd != -1);

	/* Check write succeeded */
	len = write(fd, "Hello World\n", 12);
	CU_ASSERT(len == 12);

	/* Check close succeeds */
	rc = close(fd);
	CU_ASSERT(rc == 0);

	/* Check fsync on bad fd */
	rc = close(fd);
	CU_ASSERT(rc == -1 && errno == EBADF);

	unlink(path);
}


static void
test_getpid(void)
{
	pid_t pid;

	/* Check basic operation */
	pid = getpid();
	CU_ASSERT(pid != -1);
}


static void
test_truncate(void)
{
	char *path = "testfile";
	struct stat s;
	int rc, fd;

	unlink(path);

	/* Truncate non-existent file */
	rc = truncate(path, 200);
	CU_ASSERT(rc == -1 && errno == ENOENT);

	/* Create file */
	fd = open(path, O_CREAT|O_RDWR, S_IWRITE|S_IREAD);
	CU_ASSERT(fd != -1);
	close(fd);

	/* Truncate file to size */
	rc = truncate(path, 200);
	CU_ASSERT(rc == 0);

	/* Check created size */
	rc = stat(path, &s);
	CU_ASSERT(rc == 0);
	CU_ASSERT(s.st_size == 200);

	unlink(path);

	/* Create file read-only */
	fd = open(path, O_CREAT|O_RDONLY, S_IREAD);
	CU_ASSERT(fd != -1);
	close(fd);

	/* Truncate file to size */
	rc = truncate(path, 100);
	CU_ASSERT(rc == -1 && errno == EACCES);

	unlink(path);
}


static void
test_ftruncate(void)
{
	char *path = "testfile";
	struct stat s;
	int rc, fd;

	unlink(path);

	/* Create file */
	fd = open(path, O_CREAT|O_RDWR, S_IWRITE|S_IREAD);
	CU_ASSERT(fd != -1);

	/* Increase file size */
	rc = ftruncate(fd, 200);
	CU_ASSERT(rc == 0);

	/* Check created size */
	rc = fstat(fd, &s);
	CU_ASSERT(rc == 0);
	CU_ASSERT(s.st_size == 200);
	close(fd);

	/* Open file read-only */
	fd = open(path, O_RDONLY);
	CU_ASSERT(fd != -1);

	/* Try and decrease file size */
	rc = ftruncate(fd, 100);
	CU_ASSERT(rc == -1 && (errno == EBADF || errno == EINVAL));
	close(fd);

	/* Truncate bad fd */
	rc = ftruncate(fd, 200);
	CU_ASSERT(rc == -1 && errno == EBADF);

	unlink(path);
}


static void
test_isatty(void)
{
	int rc;

	/* Check basic operation */
	rc = isatty(0);
	CU_ASSERT(rc == 1 || (rc == 0 && errno == ENOTTY));
}


static void
test_ttyname(void)
{
	char *tty;
	
	/* Check basic operation */
	tty = ttyname(0);
	CU_ASSERT(tty == NULL || strncmp(tty, "/dev/tty", 8) == 0);
}


static void
test_sleep(void)
{
	unsigned int rc;
	time_t start;
	 
	start = time(NULL);

	/* Basic operation check */
	rc = sleep(1);
	CU_ASSERT(rc < 2 && (time(NULL) - start + rc) >= 1)
}


static void
test_usleep(void)
{
	ULONGLONG start, elapsed;
	useconds_t rc;
	int i;

	start = GetTickCount64();

	/* Delay for 50 msec */
	for (i = 0; i < 10; i++) {
		rc = usleep(5000);
		CU_ASSERT(rc == 0);
	}

	/* Check delay is about right */
	elapsed = GetTickCount64() - start;
	CU_ASSERT(35 <= elapsed && elapsed <= 200);
}


static void
test_sysconf(void)
{
	long rc;

	/* Check page size is multiple of 4k */
	rc = sysconf(_SC_PAGESIZE);
	CU_ASSERT(rc && (rc % 4096) == 0);

	/* Check _SC_PAGE_SIZE */
	rc = sysconf(_SC_PAGE_SIZE);
	CU_ASSERT(rc && (rc % 4096) == 0);

	/* Check CPUs configured */
	rc = sysconf(_SC_NPROCESSORS_CONF);
	CU_ASSERT(rc >= 1);

	/* Check CPUs online */
	rc = sysconf(_SC_NPROCESSORS_ONLN);
	CU_ASSERT(rc >= 1);

	/* Check iov max */
	rc = sysconf(_SC_IOV_MAX);
	CU_ASSERT(rc > 0);
}


static void
test_fork(void)
{
	pid_t rc;

	/* Check that fork is unimplemented */
	rc = fork();
	CU_ASSERT(rc == (pid_t)-1 && errno == ENOSYS);
}


static void
test_daemon(void)
{
	int rc;

	/* Check that daemon is unimplemented */
	rc = daemon(0, 0);
	CU_ASSERT(rc == -1 && errno == ENOSYS);
}


void add_unistd_tests()
{
	CU_pSuite suite = NULL;

	suite = CU_add_suite("unistd", null_init, null_clean);

	CU_ADD_TEST(suite, test_access);
	CU_ADD_TEST(suite, test_read);
	CU_ADD_TEST(suite, test_write);
	CU_ADD_TEST(suite, test_lseek);
	CU_ADD_TEST(suite, test_unlink);
	CU_ADD_TEST(suite, test_fsync);
	CU_ADD_TEST(suite, test_close);
	CU_ADD_TEST(suite, test_getpid);
	CU_ADD_TEST(suite, test_truncate);
	CU_ADD_TEST(suite, test_ftruncate);
	CU_ADD_TEST(suite, test_isatty);
	CU_ADD_TEST(suite, test_ttyname);
	CU_ADD_TEST(suite, test_sleep);
	CU_ADD_TEST(suite, test_usleep);
	CU_ADD_TEST(suite, test_sysconf);
	CU_ADD_TEST(suite, test_fork);
	CU_ADD_TEST(suite, test_daemon);
}
