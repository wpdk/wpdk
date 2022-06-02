/*-
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 *  Copyright (c) 2022, MayaData Inc. All rights reserved.
 *  Copyright (c) 2022, DataCore Software Corporation. All rights reserved.
 * 
 *  POSIX details are based on the Open Group Base Specification Issue 7,
 *  2018 edition at https://pubs.opengroup.org/onlinepubs/9699919799/
 * 
 *  Details about Linux extensions are based on the Linux man-pages project
 *  at https://www.kernel.org/doc/man-pages/
 *
 *  Details about the MD5 implementation in cryptdll.dll are taken from
 *  http://msdn.microsoft.com/en-us/library/bb432360(v=VS.85).aspx which
 *  can be accessed through https://archive.org
 */

#ifndef _WPDK_OPENSSL_EVP_H_
#define _WPDK_OPENSSL_EVP_H_

#include <wpdk/header.h>
#include <sys/types.h>

_WPDK_BEGIN_C_HEADER

typedef struct EVP_MD_s EVP_MD;
typedef struct EVP_MD_CTX_s EVP_MD_CTX;
typedef struct ENGINE_s ENGINE;

const EVP_MD *wpdk_EVP_md5(void);
EVP_MD_CTX *wpdk_EVP_MD_CTX_create(void);
void wpdk_EVP_MD_CTX_destroy(EVP_MD_CTX *ctx);
int wpdk_EVP_DigestInit_ex(EVP_MD_CTX *ctx, const EVP_MD *type, ENGINE *impl);
int wpdk_EVP_DigestUpdate(EVP_MD_CTX *ctx, const void *data, size_t len);
int wpdk_EVP_DigestFinal_ex(EVP_MD_CTX *ctx, unsigned char *md, unsigned int *s);

#ifndef _WDPK_BUILD_LIB_
#define EVP_md5() wpdk_EVP_md5()
#define EVP_MD_CTX_create() wpdk_EVP_MD_CTX_create()
#define EVP_MD_CTX_destroy(c) wpdk_EVP_MD_CTX_destroy((c))
#define EVP_DigestInit_ex(c,type,impl) wpdk_EVP_DigestInit_ex(c,type,impl)
#define EVP_DigestUpdate(c,data,len) wpdk_EVP_DigestUpdate(c,data,len)
#define EVP_DigestFinal_ex(c,md,s) wpdk_EVP_DigestFinal_ex(c,md,s)
#endif

_WPDK_END_C_HEADER

#endif /* _WPDK_OPENSSL_EVP_H_ */
