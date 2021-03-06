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
#include <strings.h>
#include <stdio.h>

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
test_strcasecmp(void)
{
	char *s1 = "hello";
	char *s2 = "world";
	char *s3 = "HeLLO";
	char *s4 = "hELlo-WOrld";
	int rc;

	/* Check less than */
	rc = strcasecmp(s1, s2);
	CU_ASSERT(rc < 0);

	/* Check greater than */
	rc = strcasecmp(s2, s1);
	CU_ASSERT(rc > 0);

	/* Check equality */
	rc = strcasecmp(s1, s3);
	CU_ASSERT(rc == 0);

	/* Check prefix */
	rc = strcasecmp(s1, s4);
	CU_ASSERT(rc < 0);
}


static void
test_strncasecmp(void)
{
	char *s1 = "hello";
	char *s2 = "world";
	char *s3 = "HeLLO";
	char *s4 = "hELlo-WOrld";
	int rc;

	/* Check less than */
	rc = strncasecmp(s1, s2, 3);
	CU_ASSERT(rc < 0);

	/* Check equality */
	rc = strncasecmp(s1, s1, 4);
	CU_ASSERT(rc == 0);

	/* Check equality */
	rc = strncasecmp(s1, s3, 4);
	CU_ASSERT(rc == 0);

	/* Check prefix */
	rc = strncasecmp(s1, s4, 5);
	CU_ASSERT(rc == 0);
}


static void
test_strcasestr(void)
{
	char *s = "hELlo-WOrld";
	char *cp;

	/* Check exact match */
	cp = strcasestr(s, "lo-W");
	CU_ASSERT(cp == s + 3);

	/* Check case insensitive match */
	cp = strcasestr(s, "lo-w");
	CU_ASSERT(cp == s + 3);

	/* Check leading match */
	cp = strcasestr(s, "HELLO");
	CU_ASSERT(cp == s);

	/* Check trailing match */
	cp = strcasestr(s, "WORLD");
	CU_ASSERT(cp == s + 6);

	/* Check NULL argument */
	cp = strcasestr(s, NULL);
	CU_ASSERT(cp == NULL);

	/* Check empty search string */
	cp = strcasestr(s, "");
	CU_ASSERT(cp == s);
}


void add_strings_tests()
{
	CU_pSuite suite = NULL;

	suite = CU_add_suite("strings", null_init, null_clean);

	CU_ADD_TEST(suite, test_strcasecmp);
	CU_ADD_TEST(suite, test_strncasecmp);
	CU_ADD_TEST(suite, test_strcasestr);
}
