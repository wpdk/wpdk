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
#include <stdio.h>


ssize_t
wpdk_getline(char **lineptr, size_t *n, FILE *stream)
{
	// HACK - unimplemented
	WPDK_UNIMPLEMENTED();

	UNREFERENCED_PARAMETER(lineptr);
	UNREFERENCED_PARAMETER(n);
	UNREFERENCED_PARAMETER(stream);
	return 0;
}


FILE *
wpdk_fopen(const char *filename, const char *mode)
{
	char buf[MAX_PATH];
	return fopen(wpdk_get_path(filename, buf, sizeof(buf)), mode);
}


FILE *
wpdk_fdopen(int fildes, const char *mode)
{
	return _fdopen(fildes, mode);
}
