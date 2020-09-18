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

void uuid_clear(uuid_t uu);
void uuid_generate(uuid_t out);
void uuid_copy(uuid_t dst, const uuid_t src);
int uuid_compare(const uuid_t uu1, const uuid_t uu2);
int uuid_is_null(const uuid_t uu);
int uuid_parse(const char *in, uuid_t uu);
void uuid_unparse(const uuid_t uu, char *out);
void uuid_unparse_upper(const uuid_t uu, char *out);
void uuid_unparse_lower(const uuid_t uu, char *out);

_WPDK_END_C_HEADER

#endif /* _WPDK_UUID_UUID_H_ */
