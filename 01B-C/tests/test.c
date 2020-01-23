// Task 2019/01A in C

#include <stdlib.h>
#include <stdio.h>
#include <check.h>
#include <unistd.h>
#include "../src/solution.h"

START_TEST (test_boilerplate_io_read)
{
	/* reading thru a file should yield the values stored within
	 */
	char *fname = "./tests/dummy1.txt";
	ck_assert_msg( access(fname, R_OK) != -1, "Can't access dummy file.");
	
	FILE *file = fopen(fname, "rb");
	//~ FILE *file = stdin;
	// should contain "90845,1,0,2,34,12341"
	
	ck_assert_int_eq(read_module_mass(file), 90845);
	ck_assert_int_eq(read_module_mass(file), 1);
	ck_assert_int_eq(read_module_mass(file), 0);
	ck_assert_int_eq(read_module_mass(file), 2);
	ck_assert_int_eq(read_module_mass(file), 34);
	ck_assert_int_eq(read_module_mass(file), 12341);
	ck_assert_int_eq(read_module_mass(file), -1); // eof
	ck_assert_int_eq(read_module_mass(file), -1);
}
END_TEST

START_TEST (test_main_loop)
{
	/* main loop should read all fileds from file,
	 * and return the processed result
	 */
	
	char *fname = "./tests/dummy2.txt";
	ck_assert_msg( access(fname, R_OK) != -1, "Can't access dummy file.");
	
	FILE *file = fopen(fname, "rb");
	//~ FILE *file = stdin;
	// should contain "12,14,1969,100756"
	// which should result in sum(2,2,966,50346) = 51316
	
	ck_assert_int_eq(read_evaluate_all_modules(file), 51316);
}
END_TEST

START_TEST (test_mass_to_fuel_req)
{
	/* per-module calculation of fuel requirements,
	 * module-mass is the input
	 */
	
	ck_assert_int_eq(get_module_fuel_req(12), 2);
	ck_assert_int_eq(get_module_fuel_req(14), 2);
	ck_assert_int_eq(get_module_fuel_req(1969), 966);
	ck_assert_int_eq(get_module_fuel_req(100756), 50346);
	
}
END_TEST


START_TEST (test_account_fuel_reqs)
{
	/* if adding up fuel requirements of modules gives the proper sum;
	 * module-fuel-req is one input;
	 * total-fuel-req accumulator is another input
	 */
	
	int	total_fuel_req = 0;
	
	account_fuel_req(&total_fuel_req, 0);
	ck_assert_int_eq(total_fuel_req, 0);
	
	account_fuel_req(&total_fuel_req, 1);
	ck_assert_int_eq(total_fuel_req, 1);
	
	account_fuel_req(&total_fuel_req, 0);
	ck_assert_int_eq(total_fuel_req, 1);
	
	account_fuel_req(&total_fuel_req, 5);
	ck_assert_int_eq(total_fuel_req, 6);
}
END_TEST


Suite * solution_suite(void)
{
	Suite *s;
	TCase *tc_bp;
	TCase *tc_core;
	
	s = suite_create("Solution 1");
	
	/* Boilerplate test case */
	tc_bp = tcase_create("Boilerplate");
	
	tcase_add_test(tc_bp, test_boilerplate_io_read);
	suite_add_tcase(s, tc_bp);
	
	/* Core test case */
	tc_core = tcase_create("Core");
	
	tcase_add_test(tc_core, test_mass_to_fuel_req);
	tcase_add_test(tc_core, test_account_fuel_reqs);
	tcase_add_test(tc_core, test_main_loop);
	suite_add_tcase(s, tc_core);
	
	return s;
}

int main(void)
{
	int number_failed;
	Suite *s;
	SRunner *sr;
	
	s = solution_suite();
	sr = srunner_create(s);
	
	srunner_run_all(sr, CK_NORMAL);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	
	return (number_failed == 0)
		?	EXIT_SUCCESS
		:	EXIT_FAILURE
	;
}
