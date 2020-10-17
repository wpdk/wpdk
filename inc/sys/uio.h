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

#ifndef _WPDK_SYS_UIO_H_
#define _WPDK_SYS_UIO_H_

#include <wpdk/header.h>
#include <wpdk/types.h>

_WPDK_BEGIN_C_HEADER

#if defined(_MSC_VER) && !defined(_WPDK_BUILD_LIB_)
#define wpdk_writev __wrap_writev
#pragma comment(linker, "/alternatename:__wrap_writev=wpdk_writev")
#pragma comment(linker, "/alternatename:__real_writev=wpdk_writev")
#endif

struct iovec {
	void *iov_base;     /* Address */
	size_t iov_len;     /* Length */
};

ssize_t wpdk_readv(int fildes, const struct iovec *iov, int iovcnt);
ssize_t wpdk_writev(int fildes, const struct iovec *iov, int iovcnt);

#ifndef _WPDK_BUILD_LIB_
static inline ssize_t readv(int fildes, const struct iovec *iov, int iovcnt) {
	return wpdk_readv(fildes, iov, iovcnt);
}
static inline ssize_t writev(int fildes, const struct iovec *iov, int iovcnt) {
	return wpdk_writev(fildes, iov, iovcnt);
}
#endif

_WPDK_END_C_HEADER

#endif /* _WPDK_SYS_UIO_H_ */
