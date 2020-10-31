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
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

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
test_mkstemp()
{
	char path1[] = "/tmp/testXXXXXX";
	char path2[] = "/tmp/testXXXXXX";
	char path3[] = "/tmp/testXXXXXX";
	int fd1, fd2, rc;

	/* Open temporary file */
	fd1 = mkstemp(path1);
	CU_ASSERT(fd1 != -1);

	/* Check the file exists */
	rc = access(path1, F_OK);
	CU_ASSERT(rc == 0);

	/* Create next file */
	strcpy(path2, path1);
	path2[9]++;
	rc = mknod(path2, S_IFREG|S_IRWXU|S_IRWXG|S_IRWXO, 0);	
	CU_ASSERT(rc == 0);

	/* Open next temporary file */
	fd2 = mkstemp(path3);
	CU_ASSERT(fd2 != -1);
	CU_ASSERT(path1[9] != path2[9]);
	CU_ASSERT(path2[9] != path3[9]);
	CU_ASSERT(path1[9] != path3[9]);

	/* Check newly created file */
	rc = access(path3, R_OK|W_OK);
	CU_ASSERT(rc == 0);

	close(fd1);
	close(fd2);

	unlink(path1);
	unlink(path2);
	unlink(path3);
}


static void
test_rand(void)
{
	int val;

	srand((unsigned int)time(NULL));

	/* Check more than 15 bits */
	while ((rand() & ~0x7fff) == 0)
		;
	CU_ASSERT(TRUE);

	/* Check different values */
	val = rand();
	while (rand() == val)
		;
	CU_ASSERT(true);
}


static void
test_rand_r(void)
{
	unsigned int seed;
	int val;

	seed = (unsigned int)time(NULL);

	/* Check more than 15 bits */
	while ((rand_r(&seed) & ~0x7fff) == 0)
		;
	CU_ASSERT(TRUE);

	/* Check different values */
	val = rand_r(&seed);
	while (rand_r(&seed) == val)
		;
	CU_ASSERT(true);
}


static void
test_random(void)
{
	int val;

	srandom((unsigned int)time(NULL));

	/* Check more than 15 bits */
	while ((random() & ~0x7fff) == 0)
		;
	CU_ASSERT(TRUE);

	/* Check different values */
	val = random();
	while (random() == val)
		;
	CU_ASSERT(true);
}


static void
test_strtol(void)
{
	long val;

	val = strtol("903456", NULL, 10);
	CU_ASSERT(val == 903456);
}


static void
test_strtoll(void)
{
	long long val;

	val = strtoll("90345610078", NULL, 10);
	CU_ASSERT(val == 90345610078LL);
}


static void
test_malloc()
{
	char *cp;

	/* Check malloc */
	cp = malloc(100);
	CU_ASSERT(cp != NULL);

	/* Check alignment */
	CU_ASSERT(((ULONG_PTR)cp & 0xf) == 0);

	/* Check free */
	memset(cp, 0xff, 100);
	free(cp);
}


static void
test_calloc()
{
	static char buf[100];
	char *cp;

	/* Check malloc */
	cp = calloc(4, sizeof(buf) / 4);
	CU_ASSERT(cp != NULL);
	CU_ASSERT(memcmp(cp, buf, sizeof(buf)) == 0);

	/* Check alignment */
	CU_ASSERT(((ULONG_PTR)cp & 0xf) == 0);

	/* Check free */
	memset(cp, 0xff, sizeof(buf));
	free(cp);
}


static void
test_realloc()
{
	char buf[100];
	char *cp;

	/* Check malloc */
	cp = malloc(sizeof(buf));
	CU_ASSERT(cp != NULL);

	/* Check alignment */
	CU_ASSERT(((ULONG_PTR)cp & 0xf) == 0);

	/* Check realloc */
	memset(cp, 1, sizeof(buf));
	cp = realloc(cp, sizeof(buf) * 2);
	CU_ASSERT(cp != NULL);

	memset(buf, 1, sizeof(buf));
	CU_ASSERT(memcmp(cp, buf, sizeof(buf)) == 0);

	/* Check free */
	memset(cp, 0xff, sizeof(buf) * 2);
	free(cp);
}


static void
test_posix_memalign()
{
	char *cp;
	int rc;

	/* Check malloc */
	rc = posix_memalign(&cp, 256, 100);
	CU_ASSERT(rc == 0);
	CU_ASSERT(cp != NULL);

	/* Check alignment */
	CU_ASSERT(((ULONG_PTR)cp & 255) == 0);

	/* Check free */
	memset(cp, 0xff, 100);
	free(cp);
}


static void
test_free()
{
	char *cp;

	/* Check malloc */
	cp = malloc(100);
	CU_ASSERT(cp != NULL);

	/* Check alignment */
	CU_ASSERT(((ULONG_PTR)cp & 0xf) == 0);

	/* Check free */
	memset(cp, 0xff, 100);
	free(cp);

	/* Check null free */
	free(NULL);
}


void add_stdlib_tests()
{
	CU_pSuite suite = NULL;

	suite = CU_add_suite("stdlib", null_init, null_clean);

	CU_ADD_TEST(suite, test_mkstemp);
	CU_ADD_TEST(suite, test_rand);
	CU_ADD_TEST(suite, test_rand_r);
	CU_ADD_TEST(suite, test_random);
	CU_ADD_TEST(suite, test_strtol);
	CU_ADD_TEST(suite, test_strtoll);
	CU_ADD_TEST(suite, test_malloc);
	CU_ADD_TEST(suite, test_calloc);
	CU_ADD_TEST(suite, test_realloc);
	CU_ADD_TEST(suite, test_posix_memalign);
	CU_ADD_TEST(suite, test_free);
}
