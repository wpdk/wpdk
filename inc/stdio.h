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
#include_next <stdio.h>
#else
#include <../ucrt/stdio.h>
#endif

#ifndef _WPDK_STDIO_H_
#define _WPDK_STDIO_H_

#include <wpdk/types.h>
#include <stdarg.h>

_WPDK_BEGIN_C_HEADER

ssize_t wpdk_getline(char **lineptr, size_t *n, FILE *stream);
ssize_t wpdk_getdelim(char **lineptr, size_t *n, int delim, FILE *stream);
FILE *wpdk_fopen(const char *filename, const char *mode);
FILE *wpdk_fdopen(int fildes, const char *mode);
int wpdk_vdprintf(int fd, const char *format, va_list ap);
int wpdk_fileno(FILE *stream);

static inline
int dprintf(int fd, const char *format, ...)
{
	va_list ap;
	int rc;
	va_start(ap, format);
	rc = wpdk_vdprintf(fd, format, ap);
	va_end(ap);
	return rc;
}

#ifndef _WPDK_BUILD_LIB_
#define fileno(fp) wpdk_fileno(fp)
#define fdopen(fildes,mode) wpdk_fdopen(fildes,mode)
#define fopen(filename,mode) wpdk_fopen(filename,mode)
#define getline(ptr,n,stream) wpdk_getline(ptr,n,stream)
#define getdelim(ptr,n,delim,stream) wpdk_getdelim(ptr,n,delim,stream)
#define vdprintf(fd,fmt,...) wpdk_vdprintf(fd,fmt,__VA_ARGS__)
#endif

_WPDK_END_C_HEADER

#endif /* _WPDK_STDIO_H_ */
