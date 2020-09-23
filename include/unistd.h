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

#ifndef _WPDK_UNISTD_H_
#define _WPDK_UNISTD_H_

#include <wpdk/header.h>
#include <wpdk/windows.h>
#include <sys/time.h>
#include <stdio.h>
#include <io.h>

_WPDK_BEGIN_C_HEADER

#ifndef _WPDK_BUILD_LIB_
#define wpdk_unlink __wrap_unlink
#endif

#define F_OK	0
#define X_OK	1
#define W_OK	2
#define R_OK	4

#define STDIN_FILENO	0
#define STDOUT_FILENO	1
#define STDERR_FILENO	2

#define _SC_NPROCESSORS_CONF	1
#define _SC_NPROCESSORS_ONLN	2
#define _SC_PAGESIZE			3
#define _SC_PAGE_SIZE			_SC_PAGESIZE
#define _SC_IOV_MAX				4

int wpdk_access(const char *path, int amode);
int wpdk_close(int fildes);
int wpdk_daemon(int nochdir, int noclose);
pid_t wpdk_fork(void);
int wpdk_fsync(int fildes);
int wpdk_ftruncate(int fd, off_t length);
pid_t wpdk_getpid(void);
int wpdk_isatty(int fildes);
off_t wpdk_lseek(int fildes, off_t offset, int whence);
ssize_t wpdk_read(int fildes, void *buf, size_t nbyte);
unsigned wpdk_sleep(unsigned int seconds);
long wpdk_sysconf(int name);
int wpdk_truncate(const char *path, off_t length);
char *wpdk_ttyname(int fildes);
int wpdk_usleep(useconds_t useconds);
ssize_t wpdk_write(int fildes, const void *buf, size_t nbyte);

int wpdk_unlink(const char *path);

#ifndef _WPDK_BUILD_LIB_
/* Match DPDK ret_os.h to avoid macro redefinition errors later */
#define unlink _unlink
#define close _close
#define sleep(x) Sleep(1000 * (x))

#define access(path,amode) wpdk_access(path,amode)
#define _close wpdk_close
#define daemon(nochdir,noclose) wpdk_daemon(nochdir,noclose)
#define fork() wpdk_fork()
#define fsync(fd) wpdk_fsync(fd)
#define ftruncate(fd,len) wpdk_ftruncate(fd,len)
#define getpid() wpdk_getpid()
#define isatty(fd) wpdk_isatty(fd)
#undef lseek
#define lseek(fd,offset,whence) wpdk_lseek(fd,offset,whence)
static inline ssize_t read(int fildes, void *buf, size_t nbyte) {
	return wpdk_read(fildes, buf, nbyte);
}
#define Sleep(ms) wpdk_sleep((ms)/1000)
#define sysconf(name) wpdk_sysconf(name)
#define truncate(path,length) wpdk_truncate(path,length)
#define ttyname(fd) wpdk_ttyname(fd)
#define _unlink(path) wpdk_unlink(path)
#define usleep(useconds) wpdk_usleep(useconds)
static inline ssize_t write(int fildes, const void *buf, size_t nbyte) {
	return wpdk_write(fildes, buf, nbyte);
}
#endif

_WPDK_END_C_HEADER

#endif /* _WPDK_UNISTD_H_ */
