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

#ifndef _WPDK_SYS_UN_H_
#define _WPDK_SYS_UN_H_

#include <wpdk/header.h>
#include <sys/socket.h>

#ifndef __MINGW32__
#include <afunix.h>
#else
/*
 *  Define sockaddr_un since afunix.h is not present in mingw64.
 *  The structure must match the definition in the Platform SDK.
 */
struct sockaddr_un {
	sa_family_t sun_family;
	char sun_path[108];
};
#endif

#endif /* _WPDK_SYS_UN_H_ */
