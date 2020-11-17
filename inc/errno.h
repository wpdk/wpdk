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

#include <wpdk/header.h>

#ifdef _WPDK_INCLUDE_NEXT_
_WPDK_INCLUDE_NEXT_
#include_next <errno.h>
#else
#include <../ucrt/errno.h>
#endif

#ifndef _WPDK_ERRNO_H_
#define _WPDK_ERRNO_H_

#define ESHUTDOWN			200
#define EHOSTDOWN			202
#define EUSERS				203
#define EDQUOT				204
#define ESTALE				205
#define EREMOTE				206
#define EPROCLIM			207

/* Copy DPDK to avoid redefinition errors */
#define ETOOMANYREFS 10059 /* WSAETOOMANYREFS */

#endif /* _WPDK_ERRNO_H_ */
