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

_WPDK_INCLUDE_NEXT_
#include_next <sec_api/stdlib_s.h>

#if !defined(_WPDK_BUILD_LIB_) && !defined(_GLIBCXX_INCLUDE_NEXT_C_HEADERS)
#define calloc(nelem,elsize) wpdk_calloc(nelem, elsize)
#define free wpdk_free
#define malloc(size) wpdk_malloc(size)
#define realloc(ptr,size) wpdk_realloc(ptr,size)
#endif
