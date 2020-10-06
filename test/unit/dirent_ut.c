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
#include <direct.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
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
test_opendir(void)
{
	char *dirpath = "testdir";
	char path[PATH_MAX + 10];
	DIR *dir;
	int rc;

	_rmdir(dirpath);

	/* Non-existant directory */
	dir = opendir(dirpath);
	CU_ASSERT(dir == NULL && errno == ENOENT);

	/* Create directory */
	rc = _mkdir(dirpath);
	CU_ASSERT(rc == 0);

	/* Open directory */
	dir = opendir(dirpath);
	CU_ASSERT(dir != NULL);

	/* Close directory */
	rc = closedir(dir);
	CU_ASSERT(rc == 0);

	/* Path too long */
	memset(path, 'a', sizeof(path));
	path[sizeof(path)-1] = 0;
	dir = opendir(path);
	CU_ASSERT(dir == NULL && errno == ENAMETOOLONG);

	/* Null path */
	dir = opendir(NULL);
	CU_ASSERT(dir == NULL && errno == EINVAL);

	/* Empty path */
	dir = opendir("");
	CU_ASSERT(dir == NULL && errno == EINVAL);

	/* Long path */
	memset(path, 'a', sizeof(path));
	path[PATH_MAX-1] = 0;
	dir = opendir(path);
	CU_ASSERT(dir == NULL && errno == ENAMETOOLONG);

	/* Long path, non-existant dir */
	memset(path, 'a', sizeof(path));
	path[PATH_MAX-3] = 0;
	dir = opendir(path);
	CU_ASSERT(dir == NULL && errno == ENOENT);

	/* Create dir 1 */
	sprintf(path, "%s\\1", dirpath);
	rc = _mkdir(path);
	CU_ASSERT(rc == 0);

	/* Open subdirectory with '\' */
	sprintf(path, "%s\\1", dirpath);	
	dir = opendir(dirpath);
	CU_ASSERT(dir != NULL);

	/* Close directory */
	rc = closedir(dir);
	CU_ASSERT(rc == 0);

	/* Open subdirectory with '/' */
	sprintf(path, "%s/1", dirpath);	
	dir = opendir(dirpath);
	CU_ASSERT(dir != NULL);

	/* Close directory */
	rc = closedir(dir);
	CU_ASSERT(rc == 0);

	sprintf(path, "%s\\1", dirpath);
	_rmdir(path);
	_rmdir(dirpath);
}


static void
test_readdir(void)
{
	char *dirpath = "testdir";
	char path[PATH_MAX + 10];
	struct dirent *entry;
	char seen[10];
	int i, rc;
	DIR *dir;

	_rmdir(dirpath);

	/* Create directory */
	rc = _mkdir(dirpath);
	CU_ASSERT(rc == 0);

	/* Create file 1 */
	sprintf(path, "%s\\1", dirpath);
	rc = mknod(path, S_IFREG|S_IRWXU|S_IRWXG|S_IRWXO, 0);
	CU_ASSERT(rc == 0);

	/* Create file 2 */
	sprintf(path, "%s\\2", dirpath);
	rc = mknod(path, S_IFREG|S_IRWXU|S_IRWXG|S_IRWXO, 0);
	CU_ASSERT(rc == 0);

	/* Create dir 3 */
	sprintf(path, "%s\\3", dirpath);
	rc = _mkdir(path);
	CU_ASSERT(rc == 0);

	/* Open directory */
	dir = opendir(dirpath);
	CU_ASSERT(dir != NULL);

	/* Read null directory */
	entry = readdir(NULL);
	CU_ASSERT(entry == NULL && errno == EBADF);

	/* Check entries */
	_set_errno(0);
	memset(seen, 0, sizeof(seen));
	for (i = 0; (entry = readdir(dir)) != NULL; i++) {
		if (entry->d_name[0] == '.') {
			CU_ASSERT(entry->d_type == DT_DIR);
			continue;
		}
		CU_ASSERT(isdigit(entry->d_name[0]));
		seen[entry->d_name[0] - '0']++;
		CU_ASSERT(entry->d_type == (entry->d_name[0] == '3') ? DT_DIR : DT_REG);
	}

	/* Check ending conditions */
	CU_ASSERT(i == 4);
	CU_ASSERT(errno == 0);

	/* Check all entries seen */
	for (i = 0; i < (int)sizeof(seen); i++)
		CU_ASSERT(seen[i] == (i && i < 4) ? 1 : 0);

	/* Close directory */
	rc = closedir(dir);
	CU_ASSERT(rc == 0);

	/* Cleanup */
	sprintf(path, "%s\\1", dirpath);
	unlink(path);
	sprintf(path, "%s\\2", dirpath);
	unlink(path);
	sprintf(path, "%s\\3", dirpath);
	_rmdir(path);
	_rmdir(dirpath);
}


static void
test_rewinddir(void)
{
	char *dirpath = "testdir";
	char path[PATH_MAX + 10];
	struct dirent *entry;
	int i, rc;
	DIR *dir;

	_rmdir(dirpath);

	/* Create directory */
	rc = _mkdir(dirpath);
	CU_ASSERT(rc == 0);

	/* Create file 1 */
	sprintf(path, "%s\\1", dirpath);
	rc = mknod(path, S_IFREG|S_IRWXU|S_IRWXG|S_IRWXO, 0);
	CU_ASSERT(rc == 0);

	/* Create file 2 */
	sprintf(path, "%s\\2", dirpath);
	rc = mknod(path, S_IFREG|S_IRWXU|S_IRWXG|S_IRWXO, 0);
	CU_ASSERT(rc == 0);

	/* Open directory */
	dir = opendir(dirpath);
	CU_ASSERT(dir != NULL);

	/* Read all entries */
	for (i = 0; (entry = readdir(dir)) != NULL; i++)
		CU_ASSERT(entry->d_type == (entry->d_name[0] == '.') ? DT_DIR : DT_REG);
	CU_ASSERT(i == 3);

	/* Rewind null directory */
	rewinddir(NULL);

	/* rewind directory */
	rewinddir(dir);

	/* Read all entries */
	for (i = 0; (entry = readdir(dir)) != NULL; i++)
		CU_ASSERT(entry->d_type == (entry->d_name[0] == '.') ? DT_DIR : DT_REG);
	CU_ASSERT(i == 3);

	/* Close directory */
	rc = closedir(dir);
	CU_ASSERT(rc == 0);

	/* Cleanup */
	sprintf(path, "%s\\1", dirpath);
	unlink(path);
	sprintf(path, "%s\\2", dirpath);
	unlink(path);
	_rmdir(dirpath);
}


static void
test_closedir(void)
{
	DIR *dir;
	int rc;

	/* Open directory */
	dir = opendir(".");
	CU_ASSERT(dir != NULL);

	/* Close directory */
	rc = closedir(dir);
	CU_ASSERT(rc == 0);

	/* Close null directory */
	rc = closedir(NULL);
	CU_ASSERT(rc == -1 && errno == EBADF);
}


static void
test_dirfd(void)
{
	struct stat st;
	int rc, fd;
	DIR *dir;

	/* Open directory */
	dir = opendir(".");
	CU_ASSERT(dir != NULL);

	/* Test dirfd */
	fd = dirfd(dir);
	CU_ASSERT(fd != -1);

	/* Stat directory fd */
	rc = fstat(fd, &st);
	CU_ASSERT(rc == 0);
	CU_ASSERT(S_ISDIR(st.st_mode));

	/* Close directory */
	rc = closedir(dir);
	CU_ASSERT(rc == 0);
}


void add_dirent_tests()
{
	CU_pSuite suite = NULL;

	suite = CU_add_suite("dirent", null_init, null_clean);

	CU_ADD_TEST(suite, test_opendir);
	CU_ADD_TEST(suite, test_readdir);
	CU_ADD_TEST(suite, test_rewinddir);
	CU_ADD_TEST(suite, test_closedir);
	CU_ADD_TEST(suite, test_dirfd);
}
