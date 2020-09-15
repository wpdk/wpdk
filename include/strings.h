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

int strcasecmp(const char *s1, const char *s2);
int strncasecmp(const char *s1, const char *s2, size_t n);

char *strcasestr(const char *haystack, const char *needle);

_WPDK_END_C_HEADER

#endif /* _WPDK_STRINGS_H_ */
