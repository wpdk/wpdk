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

#ifdef __MINGW32__
_WPDK_INCLUDE_NEXT_
#include_next <_timeval.h>
#endif

#ifndef _WPDK__TIMEVAL_H_
#define _WPDK__TIMEVAL_H_

_WPDK_BEGIN_C_HEADER

#ifdef __MINGW32__
#define WSAtimeval timeval
#else
struct timeval {
	long tv_sec;
	long tv_usec;
};
#endif

_WPDK_END_C_HEADER

#endif /* _WPDK__TIMEVAL_H_ */
