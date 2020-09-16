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

_WPDK_BEGIN_C_HEADER

#define MD5_CBLOCK			64
#define MD5_DIGEST_LENGTH	16

// HACK - not implemented
typedef struct MD5_CTX_s { int x; } MD5_CTX;

int MD5_Init(MD5_CTX *c);
int MD5_Update(MD5_CTX *c, const void *data, size_t len);
int MD5_Final(unsigned char *md, MD5_CTX *c);
void MD5_Transform(MD5_CTX *c, const unsigned char *b);

_WPDK_END_C_HEADER

#endif /* _WPDK_OPENSSL_MD5_H_ */
