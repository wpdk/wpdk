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
#include <time.h>

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
test_rand(void)
{
	int val;

	srand((unsigned int)time(NULL));

	/* Check more than 15 bits */
	while ((rand() & ~0x7fff) == 0) ;
	CU_ASSERT(TRUE);

	/* Check different values */
	val = rand();
	while (rand() == val) ;
	CU_ASSERT(true);
}


static void
test_rand_r(void)
{
	unsigned int seed;
	int val;

	seed = (unsigned int)time(NULL);;

	/* Check more than 15 bits */
	while ((rand_r(&seed) & ~0x7fff) == 0) ;
	CU_ASSERT(TRUE);

	/* Check different values */
	val = rand_r(&seed);
	while (rand_r(&seed) == val) ;
	CU_ASSERT(true);
}


static void
test_random(void)
{
	int val;

	srandom((unsigned int)time(NULL));

	/* Check more than 15 bits */
	while ((random() & ~0x7fff) == 0) ;
	CU_ASSERT(TRUE);

	/* Check different values */
	val = random();
	while (random() == val) ;
	CU_ASSERT(true);
}


void add_stdlib_tests()
{
	CU_pSuite suite = NULL;

	suite = CU_add_suite("stdlib", null_init, null_clean);

	CU_ADD_TEST(suite, test_rand);
	CU_ADD_TEST(suite, test_rand_r);
	CU_ADD_TEST(suite, test_random);
}
