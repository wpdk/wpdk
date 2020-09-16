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

#ifndef _WPDK_SYS_STAT_H_
#define	_WPDK_SYS_STAT_H_

#include <sys/cdefs.h>
#include <sys/_types.h>
#include <../ucrt/sys/stat.h>
#include <io.h>

_WPDK_BEGIN_C_HEADER

#define S_IFMT      _S_IFMT
#define S_IFDIR     _S_IFDIR
#define S_IFCHR     _S_IFCHR
#define S_IFREG     _S_IFREG
#define S_IREAD     _S_IREAD
#define S_IWRITE    _S_IWRITE
#define S_IEXEC     _S_IEXEC

#define	S_ISDIR(m)  (((m) & S_IFMT) == S_IFDIR)
#define	S_ISCHR(m)  (((m) & S_IFMT) == S_IFCHR)
#define	S_ISREG(m)	(((m) & S_IFMT) == S_IFREG)
#define	S_ISFIFO(m) (((m) & S_IFMT) == S_IFIFO)

#define	S_ISBLK(m)	(false)
#define	S_ISLNK(m)	(false)
#define	S_ISSOCK(m)	(false)

#define S_IRWXU     (S_IREAD|S_IWRITE|S_IEXEC)
#define S_IRUSR     S_IREAD
#define S_IWUSR     S_IWRITE
#define S_IXUSR     S_IEXEC

#define S_IRWXG     (S_IREAD|S_IWRITE|S_IEXEC)
#define S_IRGRP     S_IREAD
#define S_IWGRP     S_IWRITE
#define S_IXGRP     S_IEXEC

#define S_IRWXO     (S_IREAD|S_IWRITE|S_IEXEC)
#define S_IROTH     S_IREAD
#define S_IWOTH     S_IWRITE
#define S_IXOTH     S_IEXEC

struct stat {
	dev_t	st_dev;
	ino_t	st_ino;
	mode_t	st_mode;
	nlink_t	st_nlink;
	uid_t	st_uid;
	gid_t	st_gid;
	off_t	st_size;
	time_t	st_atime;
	time_t	st_mtime;
	time_t	st_ctime;
};

int mknod(const char *path, mode_t mode, dev_t dev);
int stat(const char *path, struct stat *buf);
int fstat(int fildes, struct stat *buf);
int wpdk_chmod(const char *filename, int pmode);


#ifndef _WPDK_BUILD_LIB_
int mknod(const char *path, mode_t mode, dev_t dev);
int stat(const char *path, struct stat *buf);
int fstat(int fildes, struct stat *buf);
#define chmod(path,mode) wpdk_chmod(path,mode)
#endif


_WPDK_END_C_HEADER

#endif /* _WPDK_SYS_STAT_H_ */
