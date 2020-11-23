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

#ifndef _WPDK_DIRENT_H_
#define _WPDK_DIRENT_H_

#include <wpdk/header.h>
#include <sys/types.h>
#include <limits.h>

#define DT_REG	1
#define DT_DIR	2

typedef struct _DIR DIR;

struct dirent {
	ino_t	d_ino;
	int		d_type;
#ifdef _WPDK_BUILD_LIB_	
	char	d_name[_MAX_PATH];
#else
	char	d_name[1];
#endif
};

DIR *wpdk_opendir(const char *dirname);
int wpdk_closedir(DIR *dirp);
struct dirent *wpdk_readdir(DIR *dirp);
void wpdk_rewinddir(DIR *dirp);
int wpdk_dirfd(DIR *dirp);

#ifndef _WPDK_BUILD_LIB_
#define opendir(dirname) wpdk_opendir(dirname)
#define closedir(dirp) wpdk_closedir(dirp)
#define readdir(dirp) wpdk_readdir(dirp)
#define rewinddir(dirp) wpdk_rewinddir(dirp)
#define dirfd(dirp) wpdk_dirfd(dirp)
#endif

#endif /* _WPDK_DIRENT_H_ */
