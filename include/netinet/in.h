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

#ifndef _WPDK_NETINET_IN_H_
#define _WPDK_NETINET_IN_H_

#include <wpdk/header.h>
#include <wpdk/types.h>
#include <sys/socket.h>

typedef uint16_t in_port_t;
typedef uint32_t in_addr_t;

#define IPPROTO_IPIP	4
#define IPPROTO_GRE		47
#define IPPROTO_SCTP	132

#endif /* _WPDK_NETINET_IN_H_ */
