/*-
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 *  Copyright (c) 2020, MayaData Inc. All rights reserved.
 *  Copyright (c) 2020, DataCore Software Corporation. All rights reserved.
 */

#ifndef _WPDK_WDPKLIB_H_
#define _WPDK_WDPKLIB_H_

#define _WPDK_BUILD_LIB_

#include <sys/platform.h>
#include <sys/socket.h>

_WPDK_BEGIN_C_HEADER

int wpdk_socket_startup();

int wpdk_is_socket(int fd);
int wpdk_is_epoll(int fd);

int wpdk_socket_rc(int rc);
SOCKET wpdk_get_socket(int fd);

int wpdk_close_epoll(int fd);
int wpdk_close_socket(int socket);

ssize_t wpdk_socket_readv(int fildes, const struct iovec *iov, int iovcnt);
ssize_t wpdk_socket_writev(int fildes, const struct iovec *iov, int iovcnt);

const char *wpdk_get_path(const char *path, char *buffer, size_t len);

int wpdk_last_error();
int wpdk_last_wsa_error();
int wpdk_posix_error(int error);
int wpdk_windows_error(int error);
int wpdk_convert_to_posix(int err);

void wpdk_fatal(const char *file, int line, const char *function, const char *message);

#ifdef CHECK_UNIMPLEMENTED
#define WPDK_UNIMPLEMENTED() \
	wpdk_fatal(__FILE__,__LINE__,__FUNCTION__,"Unimplemented functionality")
#else
#define WPDK_UNIMPLEMENTED()
#endif

_WPDK_END_C_HEADER

#endif /* _WPDK_WPDKLIB_H_ */
