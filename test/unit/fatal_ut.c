#include <wpdklib.h>
#include <stdio.h>
#include <errno.h>

#include <CUnit/Basic.h>


void expect_fatal(const char *function);
int check_fatal(void);


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
check_unimplemented(void)
{
	expect_fatal(__FUNCTION__);
	CU_ASSERT(check_fatal() == 0);

	expect_fatal(__FUNCTION__);
	WPDK_UNIMPLEMENTED();
	CU_ASSERT(check_fatal() == 1);
	CU_ASSERT(check_fatal() == 0);
}


void
add_fatal_tests()
{
	CU_pSuite suite = NULL;

	suite = CU_add_suite("fatal", null_init, null_clean);

	CU_ADD_TEST(suite, check_unimplemented);
}
