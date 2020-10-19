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

#ifndef _WPDK_UUID_UUID_H_
#define _WPDK_UUID_UUID_H_

#include <wpdk/header.h>

_WPDK_BEGIN_C_HEADER

typedef unsigned char uuid_t[16];

void wpdk_uuid_clear(uuid_t uu);
void wpdk_uuid_generate(uuid_t out);
void wpdk_uuid_copy(uuid_t dst, const uuid_t src);
int wpdk_uuid_compare(const uuid_t uu1, const uuid_t uu2);
int wpdk_uuid_is_null(const uuid_t uu);
int wpdk_uuid_parse(const char *in, uuid_t uu);
void wpdk_uuid_unparse(const uuid_t uu, char *out);
void wpdk_uuid_unparse_upper(const uuid_t uu, char *out);
void wpdk_uuid_unparse_lower(const uuid_t uu, char *out);

#ifndef _WDPK_BUILD_LIB_
#define uuid_clear(uu) wpdk_uuid_clear(uu)
#define uuid_generate(out) wpdk_uuid_generate(out)
#define uuid_copy(dst,src) wpdk_uuid_copy(dst,src)
#define uuid_compare(uu1,uu2) wpdk_uuid_compare(uu1,uu2)
#define uuid_is_null(uu) wpdk_uuid_is_null(uu)
#define uuid_parse(in,uu) wpdk_uuid_parse(in,uu)
#define uuid_unparse(uu,out) wpdk_uuid_unparse(uu,out)
#define uuid_unparse_upper(uu,out) wpdk_uuid_unparse_upper(uu,out)
#define uuid_unparse_lower(uu,out) wpdk_uuid_unparse_lower(uu,out)
#endif

_WPDK_END_C_HEADER

#ifdef _MSC_VER
#pragma comment(lib, "rpcrt4")
#endif

#endif /* _WPDK_UUID_UUID_H_ */
