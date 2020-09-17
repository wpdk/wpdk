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

#ifndef _WPDK_SYS_TIME_H_
#define	_WPDK_SYS_TIME_H_

#include <wpdk/header.h>
#include <wpdk/types.h>
#include <sys/select.h>
#include <time.h>

_WPDK_BEGIN_C_HEADER

#define	CLOCK_REALTIME	1

int wpdk_clock_gettime(clockid_t clock_id, struct timespec *tp);

#ifndef _WPDK_BUILD_LIB_
#define clock_gettime(id,tp) wpdk_clock_gettime(id,tp)
#endif

_WPDK_END_C_HEADER

#endif /* _WPDK_SYS_TIME_H_ */
