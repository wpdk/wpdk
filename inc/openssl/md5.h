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

#ifndef _WPDK_OPENSSL_MD5_H_
#define _WPDK_OPENSSL_MD5_H_

#include <wpdk/header.h>
#include <wpdk/types.h>

_WPDK_BEGIN_C_HEADER

#define MD5_CBLOCK			64
#define MD5_DIGEST_LENGTH	16

typedef struct MD5_CTX_s { int x; } MD5_CTX;

int wpdk_MD5_Init(MD5_CTX *c);
int wpdk_MD5_Update(MD5_CTX *c, const void *data, size_t len);
int wpdk_MD5_Final(unsigned char *md, MD5_CTX *c);
void wpdk_MD5_Transform(MD5_CTX *c, const unsigned char *b);

#ifndef _WDPK_BUILD_LIB_
#define MD5_Init(c) wpdk_MD5_Init(c)
#define MD5_Update(c,data,len) wpdk_MD5_Update(c,data,len)
#define MD5_Final(md,c) wpdk_MD5_Final(md,c)
#define MD5_Transform(c,b) wpdk_MD5_Transform(c,b)
#endif

_WPDK_END_C_HEADER

#endif /* _WPDK_OPENSSL_MD5_H_ */
