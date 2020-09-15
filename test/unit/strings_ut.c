#include <sys/platform.h>
#include <stdio.h>

#include <CUnit/Basic.h>

#include "src/strings.c"


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
string_length(void)
{
}


void add_strings_tests()
{
	CU_pSuite suite = NULL;

	suite = CU_add_suite("strings", null_init, null_clean);

	CU_ADD_TEST(suite, string_length);
}
