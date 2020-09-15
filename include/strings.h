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

#ifndef _WPDK_STRINGS_H_
#define	_WPDK_STRINGS_H_

#include <string.h>
#include <sys/cdefs.h>

_WPDK_BEGIN_C_HEADER

int wpdk_strcasecmp(const char *s1, const char *s2);
int wpdk_strncasecmp(const char *s1, const char *s2, size_t n);
char *wpdk_strcasestr(const char *haystack, const char *needle);

#ifndef _WPDK_BUILD_LIB_
#define strcasecmp(s1,s2) wpdk_strcasecmp(s1,s2)
#define strncasecmp(s1,s2) wpdk_strncasecmp(s1,s2)
#define strcasestr(haystack,needle) wpdk_strcasecmp(haystack,needle)
#endif

_WPDK_END_C_HEADER

#endif /* _WPDK_STRINGS_H_ */
