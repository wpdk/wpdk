#include <sys/platform.h>
#include <wpdklib.h>
#include <stdlib.h>
#include <stdio.h>

#include <CUnit/Basic.h>


void add_error_tests();
void add_string_tests();
void add_strings_tests();
void add_fatal_tests();


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

	add_error_tests();
	add_string_tests();
	add_strings_tests();
	add_fatal_tests();

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
