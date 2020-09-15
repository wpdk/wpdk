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

#ifndef _WPDK_STRING_H_
#define _WPDK_STRING_H_

#include <../ucrt/string.h>
#include <sys/cdefs.h>

_WPDK_BEGIN_C_HEADER

int wpdk_strerror_r(int errnum, char *buf, size_t buflen);
char *wpdk_strerror_r_gnu(int errnum, char *buf, size_t buflen);
char *wpdk_strdup(const char *s);

#ifndef _WPDK_BUILD_LIB_
// Match DPDK ret_os.h to avoid macro redefinition errors later
#define strerror_r(a,b,c) strerror_s(b, c, a)
#define strdup(str) _strdup(str)

#if defined(__USE_GNU)
#define strerror_s(buf,len,err) wpdk_strerror_r_gnu(err,buf,len)
#else
#define strerror_s(buf,len,err) wpdk_strerror_r(err,buf,len)
#endif
#define _strdup(s) wpdk_strdup(s)
#endif

_WPDK_END_C_HEADER

#endif /* _WPDK_STRING_H_ */
