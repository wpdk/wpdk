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

#include <wpdk/header.h>
#include <wpdk/types.h>
#include <io.h>

#ifdef _WPDK_INCLUDE_NEXT_
__extension__
#include_next <sys/stat.h>
#else
#include <../ucrt/sys/stat.h>
#endif

_WPDK_BEGIN_C_HEADER

/* Match with unused bit pattern in Platform SDK */
#define _S_IFBLK	0x3000

#define S_IFMT      _S_IFMT
#define S_IFDIR     _S_IFDIR
#define S_IFCHR     _S_IFCHR
#define S_IFREG     _S_IFREG
#define S_IREAD     _S_IREAD
#define S_IWRITE    _S_IWRITE
#define S_IEXEC     _S_IEXEC
#define S_IFBLK		_S_IFBLK

#define	S_ISDIR(m)	(((m) & S_IFMT) == S_IFDIR)
#define	S_ISCHR(m)	(((m) & S_IFMT) == S_IFCHR)
#define	S_ISREG(m)	(((m) & S_IFMT) == S_IFREG)
#define	S_ISFIFO(m)	(((m) & S_IFMT) == S_IFIFO)
#define	S_ISBLK(m)	(((m) & S_IFMT) == S_IFBLK)

#define	S_ISLNK(m)	(false)
#define	S_ISSOCK(m)	(false)

#define _S_IRWXU    (_S_IREAD | _S_IWRITE | _S_IEXEC)
#define _S_IRUSR	_S_IREAD
#define _S_IWUSR	_S_IWRITE
#define _S_IXUSR	_S_IEXEC

#define S_IRWXU     _S_IRWXU
#define S_IRUSR     _S_IRUSR
#define S_IWUSR     _S_IWUSR
#define S_IXUSR     _S_IXUSR

#define S_IRWXG     (S_IRWXU >> 3)
#define S_IRGRP     (S_IRUSR >> 3)
#define S_IWGRP     (S_IWUSR >> 3)
#define S_IXGRP     (S_IXUSR >> 3)

#define S_IRWXO     (S_IRWXG >> 3)
#define S_IROTH     (S_IRGRP >> 3)
#define S_IWOTH     (S_IWGRP >> 3)
#define S_IXOTH     (S_IXGRP >> 3)

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

int wpdk_mknod(const char *path, mode_t mode, dev_t dev);
int wpdk_stat(const char *path, struct stat *buf);
int wpdk_fstat(int fildes, struct stat *buf);
int wpdk_chmod(const char *filename, int pmode);

#ifndef _WPDK_BUILD_LIB_
#define mknod(path,mode,dev) wpdk_mknod(path,mode,dev)
#define stat(path,buf) wpdk_stat(path,buf)
#define fstat(fildes,buf) wpdk_fstat(fildes,buf)
#define chmod(path,mode) wpdk_chmod(path,mode)
#endif

_WPDK_END_C_HEADER

#endif /* _WPDK_SYS_STAT_H_ */
