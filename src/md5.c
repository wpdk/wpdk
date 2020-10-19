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

#include <wpdk/internal.h>
#include <openssl/md5.h>


// HACK - MD5_CTX not implemented

int
wpdk_MD5_Init(MD5_CTX *c)
{
	// HACK - implementation
	WPDK_UNIMPLEMENTED();
	
	UNREFERENCED_PARAMETER(c);
	return 0;
}


int
wpdk_MD5_Update(MD5_CTX *c, const void *data, size_t len)
{
	// HACK - implementation
	WPDK_UNIMPLEMENTED();

	UNREFERENCED_PARAMETER(c);
	UNREFERENCED_PARAMETER(data);
	UNREFERENCED_PARAMETER(len);
	return 0;
}


int
wpdk_MD5_Final(unsigned char *md, MD5_CTX *c)
{
	// HACK - implementation
	WPDK_UNIMPLEMENTED();

	UNREFERENCED_PARAMETER(md);
	UNREFERENCED_PARAMETER(c);
	return 0;
}


unsigned char *
wpdk_MD5(const unsigned char *d, size_t n, unsigned char *md)
{
	// HACK - implementation
	WPDK_UNIMPLEMENTED();

	UNREFERENCED_PARAMETER(d);
	UNREFERENCED_PARAMETER(n);
	UNREFERENCED_PARAMETER(md);
	return NULL;
}


void
wpdk_MD5_Transform(MD5_CTX *c, const unsigned char *b)
{
	// HACK - implementation
	WPDK_UNIMPLEMENTED();

	UNREFERENCED_PARAMETER(c);
	UNREFERENCED_PARAMETER(b);
}
