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

_WPDK_BEGIN_C_HEADER

int wpdk_socket_startup(void);

int wpdk_is_socket(int fd);
int wpdk_is_epoll(int fd);

int wpdk_socket_rc(int rc);

typedef UINT_PTR SOCKET;
SOCKET wpdk_get_socket(int fd);

int wpdk_close_epoll(int fd);
int wpdk_close_socket(int socket);

struct iovec;
ssize_t wpdk_socket_readv(int fildes, const struct iovec *iov, int iovcnt);
ssize_t wpdk_socket_writev(int fildes, const struct iovec *iov, int iovcnt);

const char *wpdk_get_path(const char *path, char *buffer, size_t len);

int wpdk_last_error(void);
int wpdk_last_wsa_error(void);
int wpdk_posix_error(int error);
int wpdk_windows_error(int error);
int wpdk_convert_to_posix(int err);

void wpdk_fatal(const char *file, int line, const char *function, const char *message);

#ifdef CHECK_UNIMPLEMENTED
#define WPDK_UNIMPLEMENTED() \
	wpdk_fatal(__FILE__,__LINE__,__func__,"Unimplemented functionality")
#else
#define WPDK_UNIMPLEMENTED()
#endif

#define WPDK_FATAL_ERROR(msg)	wpdk_fatal(__FILE__,__LINE__,__func__,msg)

_WPDK_END_C_HEADER

#endif /* _WPDK_INTERNAL_H_ */
