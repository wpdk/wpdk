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

#ifndef _WPDK_HEADER_H_
#define _WPDK_HEADER_H_

#define _WPDK_INCLUDED_

#if !defined(_WIN32_WINNT)
#define WINVER			0x0602
#define _WIN32_WINNT	0x0602
#endif

#define _CRT_DECLARE_NONSTDC_NAMES	0
#define _CRT_SECURE_NO_WARNINGS		1

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#ifdef __MINGW32__
#define NO_OLDNAMES
#endif

#if defined(__GNUC__) || defined(__clang__)
#if defined(__GNUC__) && !defined(__cplusplus)
#define _WPDK_INCLUDE_NEXT_ __extension__
#else
#define _WPDK_INCLUDE_NEXT_
#endif
#ifdef __clang__
#pragma GCC diagnostic ignored "-Wgnu-include-next"
#endif
#endif

#ifdef __cplusplus
#define _WPDK_BEGIN_C_HEADER	extern "C" {
#define _WPDK_END_C_HEADER		}
#else
#define _WPDK_BEGIN_C_HEADER
#define _WPDK_END_C_HEADER
#endif

#endif /* _WPDK_HEADER_H_ */
