// Task 2019/02A in C

#include <stdlib.h>
#include <stdio.h>
#include <check.h>
#include <unistd.h>
#include "../src/solution.h"

#define ck_assert_intarr_eq(a1, a2, len) ck_assert_mem_eq(a1, a2, (len) * sizeof(int))

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

START_TEST (test_boilerplate_input_integrity)
{
	/* reading thru a file should yield the values stored within
	 */
	char *fname = "./input";
	ck_assert_msg( access(fname, R_OK) != -1, "Can't access input file.");
	
	FILE *file = fopen(fname, "rb");
	//~ FILE *file = stdin;
	
	int *tape = NULL;
	int final[157] = {1,0,0,3,1,1,2,3,1,3,4,3,1,5,0,3,2,13,1,19,1,10,19,23,1,23,9,27,1,5,27,31,2,31,13,35,1,35,5,39,1,39,5,43,2,13,43,47,2,47,10,51,1,51,6,55,2,55,9,59,1,59,5,63,1,63,13,67,2,67,6,71,1,71,5,75,1,75,5,79,1,79,9,83,1,10,83,87,1,87,10,91,1,91,9,95,1,10,95,99,1,10,99,103,2,103,10,107,1,107,9,111,2,6,111,115,1,5,115,119,2,119,13,123,1,6,123,127,2,9,127,131,1,131,5,135,1,135,13,139,1,139,10,143,1,2,143,147,1,147,10,0,99,2,0,14,0};
	
	int err = load_tape(file, &tape);
	ck_assert_int_eq(err, 0);
	ck_assert_intarr_eq(tape, final, 157);
}
END_TEST

START_TEST (test_program_init)
{
	int tape[5] = {1, 0, 0, 0, 99};
	int final[5] = {1, 12, 2, 0, 99};
	
	// presets @1 and @2 with special values
	program_init(tape);
	
	ck_assert_intarr_eq(tape, final, 5);
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
	ck_assert_intarr_eq(tape, final, 6);
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
	
	ck_assert_intarr_eq(tape, final, 9);


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
	
	ck_assert_intarr_eq(tape, final, 9);
}
END_TEST

START_TEST (test_main_loop)
{
	int tape1[5] = {1, 0, 0, 0, 99};
	int final1[5] = {2,0,0,0,99};
	computer_run(tape1);
	ck_assert_intarr_eq(tape1, final1, 5);
	
	int tape2[5] = {2,3,0,3,99};
	int final2[5] = {2,3,0,6,99};
	computer_run(tape2);
	ck_assert_intarr_eq(tape2, final2, 5);
	
	int tape3[6] = {2,4,4,5,99,0};
	int final3[6] = {2,4,4,5,99,9801};
	computer_run(tape3);
	ck_assert_intarr_eq(tape3, final3, 6);
	
	int tape4[9] = {1,1,1,4,99,5,6,0,99};
	int final4[9] = {30,1,1,4,2,5,6,0,99};
	computer_run(tape4);
	ck_assert_intarr_eq(tape4, final4, 9);
	

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
	tcase_add_test(tc_bp, test_boilerplate_input_integrity);
	// not really testing memory allocations
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
