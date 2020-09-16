#include <wpdk/internal.h>
#include <winsock2.h>
#include <stdio.h>
#include <errno.h>

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
posix_error(void)
{
	int err;

	/* Check valid error code */
	err = wpdk_posix_error(EINTR);
	CU_ASSERT(err == -1);
	CU_ASSERT(errno == EINTR);

	/* Check invalid error code */
	err = wpdk_posix_error(-1);
	CU_ASSERT(err == -1);
	CU_ASSERT(errno == -1);
}


static void
convert_to_posix(void)
{
	int err;

	/* Check WSA error code */
	err = wpdk_convert_to_posix(WSAEINTR);
	CU_ASSERT(err == EINTR);

	/* Check second WSA error code */
	err = wpdk_convert_to_posix(WSAEACCES);
	CU_ASSERT(err == EACCES);

	/* Check default is EINVAL */
	err = wpdk_convert_to_posix(ERROR_ACCOUNT_LOCKED_OUT);
	CU_ASSERT(err == EINVAL);

	/* Check success is 0 */
	err = wpdk_convert_to_posix(ERROR_SUCCESS);
	CU_ASSERT(err == 0);

	/* Check invalid error is EINVAL */
	err = wpdk_convert_to_posix(-1);
	CU_ASSERT(err == EINVAL);
}


static void
windows_error(void)
{
	int err;

	/* Check WSA error code */
	err = wpdk_windows_error(WSAEINTR);
	CU_ASSERT(err == -1);
	CU_ASSERT(errno == EINTR);

	/* Check regular error code */
	err = wpdk_windows_error(ERROR_FILE_NOT_FOUND);
	CU_ASSERT(err == -1);
	CU_ASSERT(errno == ENOENT);

	/* Check invalid error is EINVAL */
	err = wpdk_windows_error(-1);
	CU_ASSERT(err == -1);
	CU_ASSERT(errno == EINVAL);
}


static void
last_error(void)
{
	int err;

	/* Check valid error code */
	SetLastError(ERROR_FILE_NOT_FOUND);
	err = wpdk_last_error();
	CU_ASSERT(err == -1);
	CU_ASSERT(errno == ENOENT);

	/* Check invalid error is EINVAL */
	SetLastError((DWORD)-1);
	err = wpdk_last_error();
	CU_ASSERT(err == -1);
	CU_ASSERT(errno == EINVAL);
}


static void
last_wsa_error(void)
{
	int err;

	/* Check valid error code */
	WSASetLastError(WSAEINTR);
	err = wpdk_last_wsa_error();
	CU_ASSERT(err == -1);
	CU_ASSERT(errno == EINTR);

	/* Check invalid error is EINVAL */
	WSASetLastError((DWORD)-1);
	err = wpdk_last_wsa_error();
	CU_ASSERT(err == -1);
	CU_ASSERT(errno == EINVAL);
}


void
add_error_tests()
{
	CU_pSuite suite = NULL;

	suite = CU_add_suite("error", null_init, null_clean);

	CU_ADD_TEST(suite, posix_error);
	CU_ADD_TEST(suite, convert_to_posix);
	CU_ADD_TEST(suite, windows_error);
	CU_ADD_TEST(suite, last_error);
	CU_ADD_TEST(suite, last_wsa_error);
}
