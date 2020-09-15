#include <sys/platform.h>
#include <stdio.h>

#include <CUnit/Basic.h>


void add_error_tests();
void add_strings_tests();


int
main(int argc, char **argv)
{
	unsigned int	num_failures;

	UNREFERENCED_PARAMETER(argc);
	UNREFERENCED_PARAMETER(argv);

	CU_set_error_action(CUEA_ABORT);
	CU_initialize_registry();

	add_error_tests();
	add_strings_tests();

	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	num_failures = CU_get_number_of_failures();
	CU_cleanup_registry();

	return num_failures;
}
