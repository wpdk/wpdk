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

#ifndef _WPDK_NET_IF_H_
#define	_WPDK_NET_IF_H_

#include <wpdk/header.h>

_WPDK_BEGIN_C_HEADER

#define IFNAMSIZ 16

struct	ifreq {
	char ifr_name[IFNAMSIZ];
	short ifr_flags;
};

_WPDK_END_C_HEADER

#endif /* _WPDK_NET_IF_H_ */
