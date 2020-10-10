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

#ifndef _WPDK_INTERNAL_H_
#define _WPDK_INTERNAL_H_

#ifndef _WPDK_UNIT_TESTS_
#define _WPDK_BUILD_LIB_
#endif

#include <wpdk/header.h>
#include <wpdk/windows.h>
#include <wpdk/types.h>
#include <stdbool.h>

_WPDK_BEGIN_C_HEADER

void _wpdk_set_invalid_handler();
long _wpdk_invalid_handler_done;
#define wpdk_set_invalid_handler() \
	(_wpdk_invalid_handler_done || (_wpdk_set_invalid_handler(), 0))

int wpdk_socket_startup(void);

int wpdk_is_fd(int fd);
int wpdk_is_socket(int fd);
int wpdk_is_epoll(int fd);

typedef UINT_PTR SOCKET;
SOCKET wpdk_get_socket(int fd);

int wpdk_close_epoll(int fd);
int wpdk_close_socket(int socket);

struct iovec;
ssize_t wpdk_socket_read(int fildes, void *buf, size_t nbyte);
ssize_t wpdk_socket_write(int fildes, const void *buf, size_t nbyte);
ssize_t wpdk_socket_readv(int fildes, const struct iovec *iov, int iovcnt);
ssize_t wpdk_socket_writev(int fildes, const struct iovec *iov, int iovcnt);
int wpdk_socket_fcntl(int fildes, int cmd, int arg);

#define LOCKFILE_MAX ((INT64_MAX - 1) / 2)

int wpdk_lockfile(int op, int fildes, off_t start, ssize_t nbytes, bool nowait);
int wpdk_lockfile_get_range(int fildes, int whence, off_t offset,
	off_t size, off_t *pStart, off_t *pBytes);

char *wpdk_copy_path(char *buffer, size_t len, const char *path);
const char *wpdk_get_path(const char *path, char *buffer, size_t len);

int wpdk_last_error(void);
int wpdk_last_wsa_error(void);
int wpdk_windows_error(int error);
int wpdk_convert_to_posix(int err);
int wpdk_posix_error(int error);

void wpdk_warning(const char *file, int line, const char *function, const char *fmt, ...);
void wpdk_fatal(const char *file, int line, const char *function, const char *message);

#define WPDK_WARNING(...) \
	wpdk_warning(__FILE__, __LINE__, __func__, __VA_ARGS__)

#ifdef CHECK_UNIMPLEMENTED
#define WPDK_UNIMPLEMENTED() \
	wpdk_fatal(__FILE__,__LINE__,__func__,"Unimplemented functionality")
#else
#define WPDK_UNIMPLEMENTED()
#endif

#define WPDK_FATAL_ERROR(msg)	wpdk_fatal(__FILE__,__LINE__,__func__,msg)

#ifdef __MINGW32__
#define static_assert _Static_assert
#endif

_WPDK_END_C_HEADER

#endif /* _WPDK_INTERNAL_H_ */
