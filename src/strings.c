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

#include <wpdklib.h>
#include <strings.h>


int strcasecmp(const char *s1, const char *s2)
{
	return _stricmp(s1, s2);
}


int strncasecmp(const char *s1, const char *s2, size_t n)
{
	return _strnicmp(s1, s2, n);
}


char *strcasestr(const char *haystack, const char *needle)
{
	// HACK - not case insensitive
	WPDK_UNIMPLEMENTED();
	return (char *)strstr(haystack, needle);
}
