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

#ifndef	_WPDK_SYS_IOCTL_H_
#define	_WPDK_SYS_IOCTL_H_

#include <sys/cdefs.h>

_WPDK_BEGIN_C_HEADER

#define	SIOCSIFFLAGS	1
#define	SIOCGIFFLAGS	2

int wpdk_ioctl(int fd, unsigned long request, ...);

#ifndef _WPDK_BUILD_LIB_
#define ioctl(fd,req,...) wpdk_ioctl(fd,req,__VA_ARGS__)
#endif

_WPDK_END_C_HEADER

#endif /* !__WPDK_SYS_IOCTL_H_ */
