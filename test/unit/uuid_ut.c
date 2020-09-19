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
#include <uuid/uuid.h>
#include <string.h>

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


static uuid_t fill = {
	0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
	0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff
};

static uuid_t var2 = {
	0x22, 0x22, 0x11, 0x00, 0x55, 0x44, 0x77, 0x66,
	0xc8, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff
};

static uuid_t zero = { 0 };


static void
test_uuid_clear(void)
{
	uuid_t u1;

	memset(u1, 0xff, sizeof(uuid_t));

	uuid_clear(u1);

	/* Check u1 is zero */
	CU_ASSERT(memcmp(u1, zero, sizeof(uuid_t)) == 0);
}


static void
test_uuid_copy(void)
{
	uuid_t u1, u2;

	memset(u1, 0xff, sizeof(uuid_t));
	memset(u2, 0, sizeof(uuid_t));

	uuid_copy(u2, u1);

	/* Check u1 and u2 are the same */
	CU_ASSERT(memcmp(u1, u2, sizeof(uuid_t)) == 0);

	uuid_copy(u2, fill);

	/* Check u2 and fill are the same */
	CU_ASSERT(memcmp(fill, u2, sizeof(fill)) == 0);
}


static void
test_uuid_is_null(void)
{
	uuid_t u1;

	memset(u1, 0xff, sizeof(uuid_t));

	/* Check u1 is non null */
	CU_ASSERT(uuid_is_null(u1) == 0);

	/* Check u2 is null */
	CU_ASSERT(uuid_is_null(zero) != 0);

	/* Check fill is non null */
	CU_ASSERT(uuid_is_null(fill) == 0);
}


static void
test_uuid_generate(void)
{
	uuid_t u1, u2;

	memset(u1, 0xff, sizeof(uuid_t));
	memset(u2, 0, sizeof(uuid_t));

	uuid_generate(u2);

	/* Check u2 has been changed */	
	CU_ASSERT(uuid_is_null(u2) == 0);

	/* Check u2 is not the same as u1 */
	CU_ASSERT(memcmp(u1, u2, sizeof(uuid_t)) != 0);

	/* Check u2 is not the same as fill */
	CU_ASSERT(memcmp(fill, u2, sizeof(uuid_t)) != 0);

	/* Check u2 is variant 1 */
	CU_ASSERT((u2[8] & 0xc0) == 0x80);
}


static void
test_uuid_compare(void)
{
	uuid_t u1, u2;

	memset(u1, 0xff, sizeof(uuid_t));
	memset(u2, 0, sizeof(uuid_t));

	/* Check u1 is non null */
	CU_ASSERT(uuid_compare(u1, zero) != 0);

	/* Check u1 is greater */
	CU_ASSERT(uuid_compare(u1, zero) == 1);

	/* Check u1 is greater */
	CU_ASSERT(uuid_compare(zero, u1) == -1);

	/* Check u2 is null */
	CU_ASSERT(uuid_compare(u2, zero) == 0);

	/* Check fill is greater */
	CU_ASSERT(uuid_compare(fill, zero) == 1);

	/* Check varient is used */
	CU_ASSERT(uuid_compare(fill, var2) == 1);
}


static void
test_uuid_parse(void)
{
	uuid_t u;

	/* Check all zero */
	uuid_parse("00000000-0000-0000-0000-000000000000", u);
	CU_ASSERT(uuid_compare(zero, u) == 0);

	/* Check fill */
	uuid_parse("00112233-4455-6677-8899-AABBCCDDEEFF", u);
	CU_ASSERT(uuid_compare(fill, u) == 0);

	/* Check variant 2 */
	uuid_parse("00112222-4455-6677-C899-AABBCCDDEEFF", u);
	CU_ASSERT(uuid_compare(var2, u) == 0);
}


static void
test_uuid_unparse(void)
{
	char out[37];

	/* Check all zero */
	uuid_unparse(zero, out);
	CU_ASSERT(_stricmp(out, "00000000-0000-0000-0000-000000000000") == 0);

	/* Check fill */
	uuid_unparse(fill, out);
	CU_ASSERT(_stricmp(out, "00112233-4455-6677-8899-AABBCCDDEEFF") == 0);

	/* Check variant 2 */
	uuid_unparse(var2, out);
	CU_ASSERT(_stricmp(out, "00112222-4455-6677-C899-AABBCCDDEEFF") == 0);
}


static void
test_uuid_unparse_upper(void)
{
	char out[37];

	/* Check all zero */
	uuid_unparse_upper(zero, out);
	CU_ASSERT(strcmp(out, "00000000-0000-0000-0000-000000000000") == 0);

	/* Check fill */
	uuid_unparse_upper(fill, out);
	CU_ASSERT(strcmp(out, "00112233-4455-6677-8899-AABBCCDDEEFF") == 0);

	/* Check variant 2 */
	uuid_unparse_upper(var2, out);
	CU_ASSERT(strcmp(out, "00112222-4455-6677-C899-AABBCCDDEEFF") == 0);
}


static void
test_uuid_unparse_lower(void)
{
	char out[37];

	/* Check all zero */
	uuid_unparse_lower(zero, out);
	CU_ASSERT(strcmp(out, "00000000-0000-0000-0000-000000000000") == 0);

	/* Check fill */
	uuid_unparse_lower(fill, out);
	CU_ASSERT(strcmp(out, "00112233-4455-6677-8899-aabbccddeeff") == 0);

	/* Check variant 2 */
	uuid_unparse_lower(var2, out);
	CU_ASSERT(strcmp(out, "00112222-4455-6677-c899-aabbccddeeff") == 0);
}


void add_uuid_tests()
{
	CU_pSuite suite = NULL;

	suite = CU_add_suite("uuid", null_init, null_clean);

	CU_ADD_TEST(suite, test_uuid_clear);
	CU_ADD_TEST(suite, test_uuid_copy);
	CU_ADD_TEST(suite, test_uuid_is_null);
	CU_ADD_TEST(suite, test_uuid_generate);
	CU_ADD_TEST(suite, test_uuid_compare);
	CU_ADD_TEST(suite, test_uuid_parse);
	CU_ADD_TEST(suite, test_uuid_unparse);
	CU_ADD_TEST(suite, test_uuid_unparse_upper);
	CU_ADD_TEST(suite, test_uuid_unparse_lower);
}
