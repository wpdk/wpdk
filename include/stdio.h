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

#ifndef _WPDK_STDIO_H_
#define _WPDK_STDIO_H_

#include <wpdk/header.h>
#include <wpdk/windows.h>
#include <wpdk/types.h>

#ifdef _WPDK_INCLUDE_NEXT_
__extension__
#include_next <stdio.h>
#else
#include <../ucrt/stdio.h>
#endif

_WPDK_BEGIN_C_HEADER

ssize_t wpdk_getline(char **lineptr, size_t *n, FILE *stream);
FILE *wpdk_fopen(const char *filename, const char *mode);
FILE *wpdk_fdopen(int fildes, const char *mode);

#ifndef _WPDK_BUILD_LIB_
#define fdopen(fildes,mode) wpdk_fdopen(fildes,mode)
#define fopen(filename,mode) wpdk_fopen(filename,mode)
#define getline(ptr,n,stream) wpdk_getline(ptr,n,stream)
#endif

_WPDK_END_C_HEADER

#endif /* _WPDK_STDIO_H_ */
