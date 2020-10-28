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
#include <stdlib.h>
#include <stdio.h>

#include <CUnit/Basic.h>


void add_dirent_tests();
void add_error_tests(void);
void add_fatal_tests(void);
void add_fcntl_tests(void);
void add_file_tests(void);
void add_ifaddrs_tests(void);
void add_include_c_tests(void);
void add_include_cpp_tests(void);
void add_ioctl_tests(void);
void add_md5_tests(void);
void add_netdb_tests(void);
void add_pthread_tests(void);
void add_pwd_tests(void);
void add_resource_tests(void);
void add_select_tests(void);
void add_semaphore_tests(void);
void add_signal_tests(void);
void add_stat_tests(void);
void add_stdio_tests(void);
void add_stdlib_tests(void);
void add_string_tests(void);
void add_strings_tests(void);
void add_termios_tests(void);
void add_time_tests(void);
void add_uio_tests(void);
void add_uuid_tests(void);
void add_unistd_tests(void);


static int fatal_count;
static const char *fatal_function;


int
main(int argc, char **argv)
{
	unsigned int	num_failures;

	UNREFERENCED_PARAMETER(argc);
	UNREFERENCED_PARAMETER(argv);

	CU_set_error_action(CUEA_ABORT);
	CU_initialize_registry();

	add_dirent_tests();
	add_error_tests();
	add_fcntl_tests();
	add_file_tests();
	add_fatal_tests();
	add_ifaddrs_tests();
	add_include_c_tests();
	add_include_cpp_tests();
	add_ioctl_tests();
	add_md5_tests();
	add_netdb_tests();
	add_pthread_tests();
	add_pwd_tests();
	add_resource_tests();
	add_select_tests();
	add_semaphore_tests();
	add_signal_tests();
	add_stat_tests();
	add_stdio_tests();
	add_stdlib_tests();
	add_string_tests();
	add_strings_tests();
	add_termios_tests();
	add_time_tests();
	add_uio_tests();
	add_uuid_tests();
	add_unistd_tests();

	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	num_failures = CU_get_number_of_failures();
	CU_cleanup_registry();

	return num_failures;
}


void
expect_fatal (const char *function)
{
	fatal_function = function;
	fatal_count = 0;
}


int
check_fatal ()
{
	int count = fatal_count;
	fatal_function = NULL;
	fatal_count = 0;
	return count;
}


void
wpdk_fatal(const char *file, int line, const char *function, const char *message)
{
	if (!fatal_function || strcmp(function, fatal_function) != 0) {
		fprintf(stderr, "%s:%d: %s(): %s\n", file, line, function,
			(message ? message : "fatal error"));

		CU_ASSERT_FATAL(0);
	}
	
	fatal_count++;
}
