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

#ifndef _WPDK_NETDB_H_
#define _WPDK_NETDB_H_

#include <wpdk/header.h>
#include <sys/socket.h>

_WPDK_BEGIN_C_HEADER

int wpdk_getaddrinfo(const char *node, const char *service,
	const struct addrinfo *hints, struct addrinfo **res);
void wpdk_freeaddrinfo(struct addrinfo *ai);
const char *wpdk_gai_strerror(int ecode);

#ifndef _WPDK_BUILD_LIB_
#define getaddrinfo wpdk_getaddrinfo
#define freeaddrinfo wpdk_freeaddrinfo
#undef gai_strerror
#define gai_strerror wpdk_gai_strerror
#endif

_WPDK_END_C_HEADER

#endif /* _WPDK_NETDB_H_ */
