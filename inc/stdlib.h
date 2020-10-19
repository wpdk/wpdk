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
#include_next <stdlib.h>
#else
#include <../ucrt/stdlib.h>
#endif

#if !defined(_WPDK_STDLIB_H_) && !defined(_GLIBCXX_INCLUDE_NEXT_C_HEADERS)
#define _WPDK_STDLIB_H_

#include <string.h>

_WPDK_BEGIN_C_HEADER

int wpdk_mkstemp(char *path);
int wpdk_posix_memalign(void **memptr, size_t alignment, size_t size);
long int wpdk_random(void);
void wpdk_srandom(unsigned int seed);
int wpdk_rand_r(unsigned int *seedp);
void *wpdk_calloc(size_t nelem, size_t elsize);
int wpdk_rand(void);
void wpdk_srand(unsigned seed);
long wpdk_strtol(const char *str, char **endptr, int base);
long long wpdk_strtoll(const char *str, char **endptr, int base);

#ifndef _WPDK_BUILD_LIB_
#define mkstemp(path) wpdk_mkstemp(path)
#define posix_memalign(memptr,align,size) wpdk_posix_memalign(memptr,align,size)
#define random() wpdk_random()
#define srandom(seed) wpdk_srandom(seed)
#define srand(s) wpdk_srand(s)
#define rand() wpdk_rand()
#define rand_r(seedp) wpdk_rand_r(seedp)
#define strtol(str,end,base) wpdk_strtol(str,end,base)
#define strtoll(str,end,base) wpdk_strtoll(str,end,base)

#if !defined(__MINGW32__) || !defined(_INC_STDLIB_S)
#define calloc(nelem,elsize) wpdk_calloc(nelem, elsize)
#endif
#endif

_WPDK_END_C_HEADER

#endif /* _WPDK_STDLIB_H_ */
