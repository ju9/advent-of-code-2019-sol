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
	
	ck_assert_int_eq(read_code(file), 90845);
	ck_assert_int_eq(read_code(file), 1);
	ck_assert_int_eq(read_code(file), 0);
	ck_assert_int_eq(read_code(file), 2);
	ck_assert_int_eq(read_code(file), 34);
	ck_assert_int_eq(read_code(file), 12341);
	ck_assert_int_eq(read_code(file), -1); // eof
	ck_assert_int_eq(read_code(file), -1);
}
END_TEST

START_TEST (test_program_init)
{
	int tape[5] = {1, 0, 0, 0, 99};
	int final[5] = {1, 12, 2, 0, 99};
	
	// presets @1 and @2 with special values
	program_init(tape);
	
	ck_assert_mem_eq(tape, final, 5);
}
END_TEST

START_TEST (test_halt)
{
	int tape[6] = {99, 1, 1, 1, 1, 99};
	int final[6] = {99, 1, 1, 1, 1, 99};
	int pc = 0;
	int halted;
	
	// HALT @0
	halted = computer_step(tape, &pc);
	// should have halted
	ck_assert_int_eq(halted, 1);
	
	pc = 5; // HALT @5
	halted = computer_step(tape, &pc);
	// should have halted
	ck_assert_int_eq(halted, 1);
	
	// HALT shouldn't have changed anything
	ck_assert_mem_eq(tape, final, 6);
}
END_TEST

START_TEST (test_not_halt)
{
	int tape[9] = {1, 0, 0, 0, 2, 0, 0, 0, 99};
	int pc = 0;
	int halted;
	
	// ADD @0
	halted = computer_step(tape, &pc);
	// should not have halted after addition
	ck_assert_int_eq(halted, 0);
	
	pc = 4; // MULT @4
	halted = computer_step(tape, &pc);
	// should not have halted after multiplication
	ck_assert_int_eq(halted, 0);
}
END_TEST

START_TEST (test_pc_update)
{
	int tape[10] = {1, 1, 1, 1, 2, 2, 2, 2, 99, 0};
	int pc = 0;
	
	// ADD @0
	computer_step(tape, &pc);
	// should have stepped over 4 fields
	ck_assert_int_eq(pc, 4);
	
	// MULT @4
	computer_step(tape, &pc);
	// again
	ck_assert_int_eq(pc, 8);
	
	/*
	// HALT @8
	computer_step(tape, &pc);
	// again
	ck_assert_int_eq(pc, 9);
		// operation after HALT does not continue -- PC can be undefined
	*/

	pc = 1; // ADD @1
	computer_step(tape, &pc);
	// should have stepped over 4 fields
	ck_assert_int_eq(pc, 5);
	
	// MULT @5
	computer_step(tape, &pc);
	// again
	ck_assert_int_eq(pc, 9);
}
END_TEST

START_TEST (test_addition)
{
	int tape[9] = {1, 0, 0, 0, 1, 4, 8, 1, 99};
	int final[9] = {2, 100, 0, 0, 1, 4, 8, 1, 99};
	int pc = 0;
	
	computer_step(tape, &pc);
	ck_assert_int_eq(tape[0], 2);
	
	computer_step(tape, &pc);
	ck_assert_int_eq(tape[1], 100);
	
	ck_assert_mem_eq(tape, final, 9);


}
END_TEST

START_TEST (test_multiplication)
{
	int tape[9] = {2, 0, 0, 0, 2, 4, 8, 1, 99};
	int final[9] = {4, 198, 0, 0, 2, 4, 8, 1, 99};
	int pc = 0;
	
	computer_step(tape, &pc);
	ck_assert_int_eq(tape[0], 4);
	
	computer_step(tape, &pc);
	ck_assert_int_eq(tape[1], 198);
	
	ck_assert_mem_eq(tape, final, 9);
}
END_TEST

START_TEST (test_main_loop)
{
	int tape1[5] = {1, 0, 0, 0, 99};
	int final1[5] = {2,0,0,0,99};
	int pc = 0;
	computer_run(tape1, &pc);
	ck_assert_mem_eq(tape1, final1, 5);
	
	int tape2[5] = {2,3,0,3,99};
	int final2[5] = {2,3,0,6,99};
	pc = 0;
	computer_run(tape2, &pc);
	ck_assert_mem_eq(tape2, final2, 5);
	
	int tape3[6] = {2,4,4,5,99,0};
	int final3[6] = {2,4,4,5,99,9801};
	pc = 0;
	computer_run(tape3, &pc);
	ck_assert_mem_eq(tape3, final3, 6);
	
	int tape4[9] = {1,1,1,4,99,5,6,0,99};
	int final4[9] = {30,1,1,4,2,5,6,0,99};
	pc = 0;
	computer_run(tape4, &pc);
	ck_assert_mem_eq(tape4, final4, 9);
	

}
END_TEST



Suite * solution_suite(void)
{
	Suite *s;
	TCase *tc_bp;
	TCase *tc_core;
	
	s = suite_create("Solution");
	
	/* Boilerplate test case */
	tc_bp = tcase_create("Boilerplate");
	
	tcase_add_test(tc_bp, test_boilerplate_io_read);
	suite_add_tcase(s, tc_bp);
	
	
	/* Core test case */
	tc_core = tcase_create("Core");
	
	tcase_add_test(tc_core, test_program_init);
	tcase_add_test(tc_core, test_halt);
	tcase_add_test(tc_core, test_not_halt);
	tcase_add_test(tc_core, test_pc_update);
	tcase_add_test(tc_core, test_addition);
	tcase_add_test(tc_core, test_multiplication);
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
