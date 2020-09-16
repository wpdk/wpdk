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

#ifndef	_WPDK_IFADDRS_H_
#define	_WPDK_IFADDRS_H_

#include <wpdk/header.h>
#include <sys/socket.h>

_WPDK_BEGIN_C_HEADER

struct ifaddrs {
	struct ifaddrs *ifa_next;
	char *ifa_name;
	unsigned int ifa_flags;
	struct sockaddr	*ifa_addr;    
};

int wpdk_getifaddrs(struct ifaddrs **ifap);
void wpdk_freeifaddrs(struct ifaddrs *ifa);

#ifndef _WPDK_BUILD_LIB_
#define getifaddrs(ifap) wpdk_getifaddrs(ifap)
#define freeifaddrs(ifa) wpdk_freeifaddrs(ifa)
#endif

_WPDK_END_C_HEADER

#endif /* _WPDK_IFADDRS_H_ */
