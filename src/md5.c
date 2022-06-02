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
 *
 *  Details about the MD5 implementation in cryptdll.dll are taken from
 *  http://msdn.microsoft.com/en-us/library/bb432360(v=VS.85).aspx which
 *  can be accessed through https://archive.org
 */

#include <wpdk/internal.h>
#include <openssl/md5.h>
#include <openssl/evp.h>


static void (*MD5Init)(MD5_CTX *context);
static void (*MD5Update)(MD5_CTX *context, const unsigned char *data, unsigned int len);
static void (*MD5Final)(MD5_CTX *context);

static INIT_ONCE wpdk_md5_once = INIT_ONCE_STATIC_INIT;

struct EVP_MD_CTX_s {
	MD5_CTX c;
};

static const struct EVP_MD_s {
	int dummy;
} wpdk_md5;


#ifdef __MINGW32__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-function-type"
#endif

static BOOL CALLBACK
wpdk_md5_initialise(INIT_ONCE *pInit, void *arg, void **pContext)
{
	HMODULE h;

	UNREFERENCED_PARAMETER(pInit);
	UNREFERENCED_PARAMETER(arg);

	h = LoadLibrary("cryptdll.dll");

	if (h == NULL)
		WPDK_FATAL_ERROR("Unable to load cryptdll.dll");

	MD5Init = (void (*)(MD5_CTX *))GetProcAddress(h, "MD5Init");
	MD5Update = (void (*)(MD5_CTX *, const unsigned char *, unsigned int))
					GetProcAddress(h, "MD5Update");
	MD5Final = (void (*)(MD5_CTX *))GetProcAddress(h, "MD5Final");

	if (!MD5Init || !MD5Update || !MD5Final)
		WPDK_FATAL_ERROR("Unable to find MD5");

	*pContext = NULL;
	return TRUE;
}

#ifdef __MINGW32__
#pragma GCC diagnostic pop
#endif


int
wpdk_MD5_Init(MD5_CTX *c)
{
	void *context = NULL;

	/*
	 *  Initialise the function pointers.
	 *
	 *  NOTE: The documentation appears to incorrectly state the order
	 *  of the arguments as (..., Context, Parameter).  According to
	 *  the function prototype and the example code, the correct order
	 *  is (..., Parameter, Context).
	 */
	InitOnceExecuteOnce(&wpdk_md5_once, wpdk_md5_initialise,
			NULL, &context);

	(*MD5Init)(c);
	return 1;
}


int
wpdk_MD5_Update(MD5_CTX *c, const void *data, size_t len)
{
	(*MD5Update)(c, data, (unsigned int)len);
	return 1;
}


void
wpdk_MD5_Transform(MD5_CTX *c, const unsigned char *b)
{
	wpdk_MD5_Update(c, b, MD5_CBLOCK);
}


int
wpdk_MD5_Final(unsigned char *md, MD5_CTX *c)
{
	(*MD5Final)(c);

	memcpy(md, c->digest, MD5_DIGEST_LENGTH);
	return 1;
}


unsigned char *
wpdk_MD5(const unsigned char *d, unsigned long n, unsigned char *md)
{
	static unsigned char digest[MD5_DIGEST_LENGTH];
	unsigned char *buf = (md) ? md : digest;
	MD5_CTX context;

	wpdk_MD5_Init(&context);
	wpdk_MD5_Update(&context, d, n);
	wpdk_MD5_Final(buf, &context);

	return buf;
}


const EVP_MD *
wpdk_EVP_md5(void)
{
	return &wpdk_md5;
}


EVP_MD_CTX *
wpdk_EVP_MD_CTX_create(void)
{
	EVP_MD_CTX *c = calloc(1, sizeof(EVP_MD_CTX));

	if (c == NULL)
		wpdk_posix_error(ENOMEM);

	return c;
}


void
wpdk_EVP_MD_CTX_destroy(EVP_MD_CTX *ctx)
{
	free(ctx);
}


int
wpdk_EVP_DigestInit_ex(EVP_MD_CTX *ctx, const EVP_MD *type, ENGINE *impl)
{
	UNREFERENCED_PARAMETER(impl);

	if (!ctx) {
		wpdk_posix_error(EINVAL);
		return 0;
	}

	if (type != &wpdk_md5) {
		WPDK_UNIMPLEMENTED();
		return 0;
	}

	return (wpdk_MD5_Init(&ctx->c) == 1) ? 1 : 0;
}


int
wpdk_EVP_DigestUpdate(EVP_MD_CTX *ctx, const void *data, size_t len)
{
	return wpdk_MD5_Update(&ctx->c, data, len);
}


int
wpdk_EVP_DigestFinal_ex(EVP_MD_CTX *ctx, unsigned char *md, unsigned int *s)
{
	UNREFERENCED_PARAMETER(s);
	return wpdk_MD5_Final(md, &ctx->c);
}
