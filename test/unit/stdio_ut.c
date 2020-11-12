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
#include <inttypes.h>
#include <stdlib.h>
#include <unistd.h>
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
test_getline(void)
{
	char *path = "testfile";
	char *buf = NULL;
	size_t len = 0;
	ssize_t rc;
	int i, j;
	FILE *fp;

	unlink(path);

	/* Open test file */
	fp = fopen(path, "w+");
	CU_ASSERT(fp != NULL);

	for (i = 1; i < 10; i++) {
		for (j = 0; j < (BUFSIZ + 10) * i; j++)
			putc('a' + i, fp);
		putc('\n', fp);
	}

	fputs("tail", fp);
	fflush(fp);
	rewind(fp);

	/* Check with no buffer */
	rc = getline(&buf, &len, fp);
	CU_ASSERT(rc == BUFSIZ + 11);
	CU_ASSERT(buf != NULL);
	CU_ASSERT(len > (size_t)rc);
	CU_ASSERT((size_t)rc == strlen(buf));
	CU_ASSERT(buf[rc - 1] == '\n');
	CU_ASSERT(buf[rc] == 0);

	/* Check all lines */
	for (i = 2; i < 10; i++) {
		rc = getline(&buf, &len, fp);
		CU_ASSERT(rc == (ssize_t)(BUFSIZ + 10) * i + 1);
		CU_ASSERT(buf != NULL);
		CU_ASSERT(len > (size_t)rc);
		CU_ASSERT((size_t)rc == strlen(buf));
		CU_ASSERT(buf[rc - 1] == '\n');
		CU_ASSERT(buf[rc] == 0);
	}

	/* Check tail */
	rc = getline(&buf, &len, fp);
	CU_ASSERT(rc == 4);

	/* Check EOF */
	rc = getline(&buf, &len, fp);
	CU_ASSERT(rc == -1);
	CU_ASSERT(feof(fp));

	fclose(fp);
	unlink(path);
}


static void
test_getdelim(void)
{
	char *path = "testfile";
	char *buf = NULL;
	size_t len = 0;
	ssize_t rc;
	int i, j;
	FILE *fp;

	unlink(path);

	/* Open test file */
	fp = fopen(path, "w+");
	CU_ASSERT(fp != NULL);

	for (i = 1; i < 10; i++) {
		for (j = 0; j < i; j++)
			putc('a' + i, fp);
		putc('-', fp);
	}

	fputs("tail", fp);
	fflush(fp);
	rewind(fp);

	/* Check with no buffer */
	rc = getdelim(&buf, &len, '-', fp);
	CU_ASSERT(rc == 2);
	CU_ASSERT(buf != NULL);
	CU_ASSERT(len > (size_t)rc);
	CU_ASSERT((size_t)rc == strlen(buf));
	CU_ASSERT(buf[rc - 1] == '-');
	CU_ASSERT(buf[rc] == 0);

	/* Check all lines */
	for (i = 2; i < 10; i++) {
		rc = getdelim(&buf, &len, '-', fp);
		CU_ASSERT(rc == i + 1);
		CU_ASSERT(buf != NULL);
		CU_ASSERT(len > (size_t)rc);
		CU_ASSERT((size_t)rc == strlen(buf));
		CU_ASSERT(buf[rc - 1] == '-');
		CU_ASSERT(buf[rc] == 0);
	}

	/* Check tail */
	rc = getdelim(&buf, &len, '-', fp);
	CU_ASSERT(rc == 4);

	/* Check EOF */
	rc = getdelim(&buf, &len, '-', fp);
	CU_ASSERT(rc == -1);
	CU_ASSERT(feof(fp));

	fclose(fp);
	unlink(path);
}


static void
test_fopen(void)
{
	char *path = "testfile";
	FILE *fp;
	int rc;

	unlink(path);

	/* Open test file */
	fp = fopen(path, "w+");
	CU_ASSERT(fp != NULL);
	fclose(fp);

	/* Check it was created */
	rc = access(path, W_OK|R_OK);
	CU_ASSERT(rc == 0);

	unlink(path);
}


static void
test_fdopen(void)
{
	char *path = "testfile";
	struct stat s;
	int rc, fd;
	FILE *fp;

	unlink(path);

	/* Create test file */
	fd = open(path, O_CREAT|O_RDWR, S_IWRITE|S_IREAD);
	CU_ASSERT(fd != -1);

	/* Open file handle */
	fp = fdopen(fd, "w+");
	CU_ASSERT(fp != NULL);

	/* Write test string */
	rc = fputs("Hello World", fp);
	CU_ASSERT(rc >= 0);
	fclose(fp);

	/* Check file size */
	rc = stat(path, &s);
	CU_ASSERT(rc == 0);
	CU_ASSERT(s.st_size == 11);

	unlink(path);
}


static void
test_fileno(void)
{
	char *path = "testfile";
	FILE *fp;
	int fd;
	
	/* Check standard I/O handles */
	CU_ASSERT(fileno(stdin) == 0);
	CU_ASSERT(fileno(stdout) == 1);
	CU_ASSERT(fileno(stderr) == 2);

	unlink(path);

	/* Create test file */
	fd = open(path, O_CREAT|O_RDWR, S_IWRITE|S_IREAD);
	CU_ASSERT(fd != -1);

	/* Open file handle */
	fp = fdopen(fd, "w+");
	CU_ASSERT(fp != NULL);

	/* Check fileno */
	CU_ASSERT(fileno(fp) == fd);

	fclose(fp);
	unlink(path);
}


static void
test_vdprintf(void)
{
	char *path = "testfile";
	char buf[200];
	ssize_t rc;
	int fd;

	unlink(path);

	/* Create test file */
	fd = open(path, O_CREAT|O_RDWR, S_IWRITE|S_IREAD);
	CU_ASSERT(fd != -1);

	/* Write initial message */
	rc = write(fd, "Initial Write! ", 15);
	CU_ASSERT(rc == 15);

	/* dprintf message */
	rc = dprintf(fd, "Hello %s! ", "World", 13);
	CU_ASSERT(rc == 13);

	/* Write final message */
	rc = write(fd, "Final Write!", 12);
	CU_ASSERT(rc == 12);

	/* Rewind */
	rc = lseek(fd, 0, SEEK_SET);
	CU_ASSERT(rc == 0);

	/* Read file contents */
	rc = read(fd, buf, sizeof(buf));
	CU_ASSERT(rc == 40);
	CU_ASSERT(strncmp(buf, "Initial Write! Hello World! Final Write!", 40) == 0);

	close(fd);
	unlink(path);
}


static void
test_sprintf(void)
{
	char s[200];
	int64_t v;
	int rc;

	/* Check %z is supported */
	rc = sprintf(s, "%zd", sizeof(s));
	CU_ASSERT(rc == 3);
	CU_ASSERT(strlen(s) == 3);
	CU_ASSERT(strcmp(s, "200") == 0);

	/* Check PRId64 is supported */
	v = 999;
	rc = sprintf(s, "%" PRId64, v);
	CU_ASSERT(rc == 3);
	CU_ASSERT(strlen(s) == 3);
	CU_ASSERT(strcmp(s, "999") == 0);
}


void
add_stdio_tests()
{
	CU_pSuite suite = NULL;

	suite = CU_add_suite("stdio", null_init, null_clean);

	CU_ADD_TEST(suite, test_getline);
	CU_ADD_TEST(suite, test_getdelim);
	CU_ADD_TEST(suite, test_fopen);
	CU_ADD_TEST(suite, test_fdopen);
	CU_ADD_TEST(suite, test_fileno);
	CU_ADD_TEST(suite, test_vdprintf);
	CU_ADD_TEST(suite, test_sprintf);
}
