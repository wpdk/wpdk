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

#ifndef _WPDK_ERRNO_H_
#define _WPDK_ERRNO_H_

#include <wpdk/header.h>

#ifdef _WPDK_INCLUDE_NEXT_
__extension__
#include_next <errno.h>
#else
#include <../ucrt/errno.h>
#endif

#define _WPDK_BASE_ERRNO	200
#define ESHUTDOWN			200
#define ETOOMANYREFS		201
#define EHOSTDOWN			202
#define EUSERS				203
#define EDQUOT				204
#define ESTALE				205
#define EREMOTE				206
#define EPROCLIM			207

#endif /* _WPDK_ERRNO_H_ */
