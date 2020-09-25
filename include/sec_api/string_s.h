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
#include_next <sec_api/string_s.h>

#ifndef _WPDK_BUILD_LIB_
#if defined(__USE_GNU)
#define strerror_s(buf,len,err) wpdk_strerror_r_gnu(err,buf,len)
#else
#define strerror_s(buf,len,err) wpdk_strerror_r(err,buf,len)
#endif
#endif
