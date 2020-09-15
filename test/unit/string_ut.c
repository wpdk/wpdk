#include <sys/platform.h>
#include <stdlib.h>
#include <stdio.h>

#include <CUnit/Basic.h>

#include "src/string.c"


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
test_strerror_r(void)
{
	char buf[1024];
	int rc;

	/* Check valid error code */
	strcpy(buf, "");
	rc = wpdk_strerror_r(EINVAL, buf, sizeof(buf));
	CU_ASSERT(rc == 0);
	CU_ASSERT(strlen(buf) != 0);

	/* Check additional error codes */
	strcpy(buf, "");
	rc = wpdk_strerror_r(ESHUTDOWN, buf, sizeof(buf));
	CU_ASSERT(rc == 0);
	CU_ASSERT(strstr(buf, "shut") != 0);

	/* Check last additional error codes */
	strcpy(buf, "");
	rc = wpdk_strerror_r(EPROCLIM, buf, sizeof(buf));
	CU_ASSERT(rc == 0);
	CU_ASSERT(strstr(buf, "proc") != 0);

	/* Check buffer length is used */
	strcpy(buf, "");
	rc = wpdk_strerror_r(EINVAL, buf, 3);
	CU_ASSERT(strcmp(buf, "In") == 0);
}


static void
test_strerror_r_gnu(void)
{
	char buf[1024];
	char *s;

	/* Check valid error code */
	strcpy(buf, "");
	s = wpdk_strerror_r_gnu(EINVAL, buf, sizeof(buf));
	CU_ASSERT(s == buf);
	CU_ASSERT(strlen(buf) != 0);
}


static void
test_strdup(void)
{
	char *s, *msg = "Test Message";

	s = strdup(msg);
	CU_ASSERT(s != msg);
	CU_ASSERT(strcmp(s, msg) == 0);

	free(s);
}


void add_string_tests()
{
	CU_pSuite suite = NULL;

	suite = CU_add_suite("string", null_init, null_clean);

	CU_ADD_TEST(suite, test_strerror_r);
	CU_ADD_TEST(suite, test_strerror_r_gnu);
	CU_ADD_TEST(suite, test_strdup);
}
