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
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <openssl/md5.h>
#include <openssl/evp.h>

#include <CUnit/Basic.h>


static int
null_init(void)
{
	return 0;
}


static int
null_clean(void)
{
	return 0;
}


static void
test_md5(void)
{
	static unsigned char md5[] = {
		0xED, 0x07, 0x62, 0x87, 0x53, 0x2E, 0x86, 0x36,
		0x5E, 0x84, 0x1E, 0x92, 0xBF, 0xC5, 0x0D, 0x8C
	};
	const unsigned char s[] = "Hello World!";
	unsigned char buf[MD5_DIGEST_LENGTH];
	unsigned char *cp;
	MD5_CTX c;
	int rc;

	/* Check initialisation */
	rc = MD5_Init(&c);
	CU_ASSERT(rc == 1);

	/* Check update */
	rc = MD5_Update(&c, s, 12);
	CU_ASSERT(rc == 1);

	/* Check final */
	memset(buf, 0, sizeof(buf));
	rc = MD5_Final(buf, &c);
	CU_ASSERT(rc == 1);
	CU_ASSERT(memcmp(buf, md5, sizeof(md5)) == 0);

	/* Check calculate */
	memset(buf, 0, sizeof(buf));
	cp = MD5(s, 12, buf);
	CU_ASSERT(cp == buf);
	CU_ASSERT(memcmp(buf, md5, sizeof(md5)) == 0);
}


static void
test_evp_md5(void)
{
	static unsigned char md5[] = {
		0xED, 0x07, 0x62, 0x87, 0x53, 0x2E, 0x86, 0x36,
		0x5E, 0x84, 0x1E, 0x92, 0xBF, 0xC5, 0x0D, 0x8C
	};
	const unsigned char s[] = "Hello World!";
	unsigned char buf[MD5_DIGEST_LENGTH];
	EVP_MD_CTX *ctx;
	int rc;

	/* Check create */
	ctx = EVP_MD_CTX_create();
	CU_ASSERT(ctx != NULL);

	/* Check initialisation */
	rc = EVP_DigestInit_ex(ctx, EVP_md5(), NULL);
	CU_ASSERT(rc == 1);

	/* Check update */
	rc = EVP_DigestUpdate(ctx, s, 12);
	CU_ASSERT(rc == 1);

	/* Check final */
	memset(buf, 0, sizeof(buf));
	rc = EVP_DigestFinal_ex(ctx, buf, NULL);
	CU_ASSERT(rc == 1);
	CU_ASSERT(memcmp(buf, md5, sizeof(md5)) == 0);

	/* Check destroy */
	EVP_MD_CTX_destroy(ctx);
}


void add_md5_tests()
{
	CU_pSuite suite = NULL;

	suite = CU_add_suite("md5", null_init, null_clean);

	CU_ADD_TEST(suite, test_md5);
	CU_ADD_TEST(suite, test_evp_md5);
}
