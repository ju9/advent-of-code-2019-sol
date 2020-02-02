// Task 2019/03B in C

#include <stdlib.h>
#include <stdio.h>
#include <check.h>
#include <unistd.h>
#include "../src/solution.h"


START_TEST (test_boilerplate_list)
{
	/* Testing singly-linked list (crash-test, not much else to do here)
	 */
	ASeg dummy_payload = { .x1 = 0, .y1 = 0, .x2 = 0, .y2 = 0 };
	Litem *list = list_new(dummy_payload);
	ck_assert_ptr_null(list->next);
	
	list->next = list_new(dummy_payload);
	
	ck_assert_ptr_null(list->next->next);
	
	list_drop(&list);
	ck_assert_ptr_null(list);
	
}
END_TEST


START_TEST (test_boilerplate_io_read)
{
	/* reading thru a file should yield the values stored within
	 */
	char *fname = "./tests/dummy1.txt";
	ck_assert_msg( access(fname, R_OK) != -1, "Can't access dummy file.");
	
	FILE *file = fopen(fname, "rb");
	//~ FILE *file = stdin;
	// should contain "R75,D30,R83,U83,L12,D49,R71,U7,L72
	//		U62,R66,U55,R34,D71,R55,D58,R83"
	
	char code_buffer[8];
	int status;
	#define io_read_ck(ref_status, ref_code) \
		status = read_segment_code(file, code_buffer); \
			ck_assert(strcmp(code_buffer, ref_code) == 0); \
			ck_assert_int_eq(status, ref_status); 
	
	io_read_ck(READ_OK, "R75");
	io_read_ck(READ_OK, "D30");
	io_read_ck(READ_OK, "R83");
	io_read_ck(READ_OK, "U83");
	io_read_ck(READ_OK, "L12");
	io_read_ck(READ_OK, "D49");
	io_read_ck(READ_OK, "R71");
	io_read_ck(READ_OK, "U7");
	io_read_ck(READ_EOL, "L72");
	
	io_read_ck(READ_OK, "U62");
	io_read_ck(READ_OK, "R66");
	io_read_ck(READ_OK, "U55");
	io_read_ck(READ_OK, "R34");
	io_read_ck(READ_OK, "D71");
	io_read_ck(READ_OK, "R55");
	io_read_ck(READ_OK, "D58");
	io_read_ck(READ_EOF, "R83");
	
	io_read_ck(READ_ERR, "R83");
		
}
END_TEST

START_TEST (test_parse_segment)
{
	/* Parse segment-code into rel-segment struct
	 */
	
	RSeg rseg;
	rseg = parse_segment("U0");
		ck_assert_int_eq(rseg.dir, DIR_UP);
		ck_assert_int_eq(rseg.len, 0);
	rseg = parse_segment("U14");
		ck_assert_int_eq(rseg.dir, DIR_UP);
		ck_assert_int_eq(rseg.len, 14);
	rseg = parse_segment("L6");
		ck_assert_int_eq(rseg.dir, DIR_LEFT);
		ck_assert_int_eq(rseg.len, 6);
	rseg = parse_segment("R428");
		ck_assert_int_eq(rseg.dir, DIR_RIGHT);
		ck_assert_int_eq(rseg.len, 428);
	rseg = parse_segment("D67");
		ck_assert_int_eq(rseg.dir, DIR_DOWN);
		ck_assert_int_eq(rseg.len, 67);
	rseg = parse_segment("L0");
		ck_assert_int_eq(rseg.dir, DIR_LEFT);
		ck_assert_int_eq(rseg.len, 0);
		
	rseg = parse_segment("L-45"); // bad! but probbly permissible
		ck_assert_int_eq(rseg.dir, DIR_LEFT);
		ck_assert_int_eq(rseg.len, -45);
		
	rseg = parse_segment("X1"); // bad!
		ck_assert_int_eq(rseg.dir, DIR_INVALID);
		ck_assert_int_eq(rseg.len, 0);
	
}
END_TEST

START_TEST (test_get_absolute_segment)
{
	/* Parse segment-code into rel-segment struct
	 */
	
	ASeg aseg = { .x1 = 0, .y1 = 0, .x2 = 0, .y2 = 0 };
	aseg = get_absolute_segment((RSeg) { .dir = DIR_UP, .len = 0 }, aseg.x2, aseg.y2);
		ck_assert_int_eq(aseg.x1, 0);
		ck_assert_int_eq(aseg.y1, 0);
		ck_assert_int_eq(aseg.x2, 0);
		ck_assert_int_eq(aseg.y2, 0);
	aseg = get_absolute_segment((RSeg) { .dir = DIR_UP, .len = 3 }, aseg.x2, aseg.y2);
		ck_assert_int_eq(aseg.x1, 0);
		ck_assert_int_eq(aseg.y1, -1);
		ck_assert_int_eq(aseg.x2, 0);
		ck_assert_int_eq(aseg.y2, -3);
	aseg = get_absolute_segment((RSeg) { .dir = DIR_RIGHT, .len = 365 }, aseg.x2, aseg.y2);
		ck_assert_int_eq(aseg.x1, 1);
		ck_assert_int_eq(aseg.y1, -3);
		ck_assert_int_eq(aseg.x2, 365);
		ck_assert_int_eq(aseg.y2, -3);
	aseg = get_absolute_segment((RSeg) { .dir = DIR_DOWN, .len = 12 }, aseg.x2, aseg.y2);
		ck_assert_int_eq(aseg.x1, 365);
		ck_assert_int_eq(aseg.y1, -2);
		ck_assert_int_eq(aseg.x2, 365);
		ck_assert_int_eq(aseg.y2, 9);
	aseg = get_absolute_segment((RSeg) { .dir = DIR_LEFT, .len = 112 }, aseg.x2, aseg.y2);
		ck_assert_int_eq(aseg.x1, 364);
		ck_assert_int_eq(aseg.y1, 9);
		ck_assert_int_eq(aseg.x2, 253);
		ck_assert_int_eq(aseg.y2, 9);
}
END_TEST

START_TEST (test_read_wire)
{
	/* read one whole wire (all its segments) and parse them
	 */
	
	char *fname = "./tests/dummy1.txt";
	ck_assert_msg( access(fname, R_OK) != -1, "Can't access dummy file.");
	
	FILE *file = fopen(fname, "rb");
	// should contain "R75,D30,R83,U83,L12,D49,R71,U7,L72
	//		U62,R66,U55,R34,D71,R55,D58,R83"
	
	#define ck_assert_aseg_eq(aseg, rx1, ry1, rx2, ry2) \
		ck_assert_int_eq(aseg.x1, rx1); \
		ck_assert_int_eq(aseg.y1, ry1); \
		ck_assert_int_eq(aseg.x2, rx2); \
		ck_assert_int_eq(aseg.y2, ry2);
	
	Litem *wire = read_wire(file);
	Litem *current = wire;
		ck_assert_aseg_eq(current->seg, 1, 0, 75, 0);
		ck_assert_ptr_nonnull(current->next);
	current = current->next;
		ck_assert_aseg_eq(current->seg, 75, 1, 75, 30);
		ck_assert_ptr_nonnull(current->next);
	current = current->next;
		ck_assert_aseg_eq(current->seg, 76, 30, 158, 30);
		ck_assert_ptr_nonnull(current->next);
	current = current->next;
		ck_assert_aseg_eq(current->seg, 158, 29, 158, -53);
		ck_assert_ptr_nonnull(current->next);
	current = current->next;
		ck_assert_aseg_eq(current->seg, 157, -53, 146, -53);
		ck_assert_ptr_nonnull(current->next);
	current = current->next;
		ck_assert_aseg_eq(current->seg, 146, -52, 146, -4);
		ck_assert_ptr_nonnull(current->next);
	current = current->next;
		ck_assert_aseg_eq(current->seg, 147, -4, 217, -4);
		ck_assert_ptr_nonnull(current->next);
	current = current->next;
		ck_assert_aseg_eq(current->seg, 217, -5, 217, -11);
		ck_assert_ptr_nonnull(current->next);
	current = current->next;
		ck_assert_aseg_eq(current->seg, 216, -11, 145, -11);
		ck_assert_ptr_null(current->next);
	current = current->next;
	list_drop(&wire);
	
}
END_TEST

START_TEST (test_read_all_wires)
{
	char *fname = "./tests/dummy2.txt";
	ck_assert_msg( access(fname, R_OK) != -1, "Can't access dummy file.");
	
	FILE *file = fopen(fname, "rb");
	// should contain "R75,D30,R83,U83,L12,D49,R71,U7,L72
	//		U62,R66,U55,R34,D71,R55,D58,R83"
	
	Litem *wire1 = read_wire(file);
	ck_assert_ptr_nonnull(wire1);
	list_drop(&wire1);
	Litem *wire2 = read_wire(file);
	ck_assert_ptr_nonnull(wire2);
	list_drop(&wire2);
	Litem *wire3 = read_wire(file);
	ck_assert_ptr_null(wire3);
}
END_TEST

START_TEST (test_compare_wires)
{
	char *fname = "./tests/dummy2.txt";
	ck_assert_msg( access(fname, R_OK) != -1, "Can't access dummy file.");
	
	FILE *file = fopen(fname, "rb");
	// should contain "R8,U5,L5,D3
	//		U7,R6,D4,L4"
	
	Litem *wire1 = read_wire(file);
	Litem *wire2 = read_wire(file);
	
	int length = get_shortest_loop_length(wire1, wire2);
	ck_assert_int_eq(length, 30);
	list_drop(&wire1);
	list_drop(&wire2);
	
}
END_TEST

START_TEST (test_compare_wires2)
{
	char *fname = "./tests/dummy1.txt";
	ck_assert_msg( access(fname, R_OK) != -1, "Can't access dummy file.");
	
	FILE *file = fopen(fname, "rb");
	
	Litem *wire1 = read_wire(file);
	Litem *wire2 = read_wire(file);
	
	int length = get_shortest_loop_length(wire1, wire2);
	ck_assert_int_eq(length, 610);
	list_drop(&wire1);
	list_drop(&wire2);
	
}
END_TEST

START_TEST (test_compare_wires3)
{
	char *fname = "./tests/dummy3.txt";
	ck_assert_msg( access(fname, R_OK) != -1, "Can't access dummy file.");
	
	FILE *file = fopen(fname, "rb");
	
	Litem *wire1 = read_wire(file);
	Litem *wire2 = read_wire(file);
	
	int length = get_shortest_loop_length(wire1, wire2);
	ck_assert_int_eq(length, 410);
	list_drop(&wire1);
	list_drop(&wire2);
	
}
END_TEST

START_TEST (test_seg_length)
{
	ck_assert_int_eq(get_length(0, 0, 0, 0), 1);
	ck_assert_int_eq(get_length(5, 5, 5, 5), 1);
	ck_assert_int_eq(get_length(1, 1, 5, 1), 5);
	ck_assert_int_eq(get_length(1, 1, 1, 5), 5);
	ck_assert_int_eq(get_length(8, 1, 1, 1), 8);
	ck_assert_int_eq(get_length(1, -5, 1, 1), 7);
	
	ck_assert_int_eq(get_seg_length(
		(ASeg) { .x1 = 1, .y1 = 1, .x2 = 5, .y2 = 1 }
	), 5);
}
END_TEST

START_TEST (test_find_intersect)
{
	int pos, found;
	
	found = find_collinear_intersect(0, 5, 6, 1, &pos);
	ck_assert_int_eq(found, 1);
	ck_assert_int_eq(pos, 1);
	
	found = find_collinear_intersect(5, 1, 6, 1, &pos);
	ck_assert_int_eq(found, 1);
	ck_assert_int_eq(pos, 5);
	
	found = find_collinear_intersect(5, 1, 3, 1, &pos);
	ck_assert_int_eq(found, 1);
	ck_assert_int_eq(pos, 3);
	
	found = find_collinear_intersect(5, 1, 8, 6, &pos);
	ck_assert_int_eq(found, 0);
	//~ ck_assert_int_eq(pos, 3);
	
}
END_TEST

START_TEST (test_find_intersect2)
{
	int ix, iy, found;
	
	found = find_intersect(
		(ASeg) { .x1 = 0, .y1 = 2, .x2 = 5, .y2 = 2 },
		(ASeg) { .x1 = 3, .y1 = 5, .x2 = 3, .y2 = 0 },
		&ix, &iy
	);
	ck_assert_int_eq(found, 1);
	ck_assert_int_eq(ix, 3);
	ck_assert_int_eq(iy, 2);
	
	found = find_intersect(
		(ASeg) { .x1 = 3, .y1 = 5, .x2 = 3, .y2 = 0 },
		(ASeg) { .x1 = 0, .y1 = 2, .x2 = 5, .y2 = 2 },
		&ix, &iy
	);
	ck_assert_int_eq(found, 1);
	ck_assert_int_eq(ix, 3);
	ck_assert_int_eq(iy, 2);
	
	found = find_intersect(
		(ASeg) { .x1 = 0, .y1 = 0, .x2 = 5, .y2 = 0 },
		(ASeg) { .x1 = 0, .y1 = 1, .x2 = 5, .y2 = 1 },
		&ix, &iy
	);
	ck_assert_int_eq(found, 0);
	//~ ck_assert_int_eq(ix, 0);
	//~ ck_assert_int_eq(iy, 0);
	
	found = find_intersect(
		(ASeg) { .x1 = 0, .y1 = 0, .x2 = 0, .y2 = 5 },
		(ASeg) { .x1 = 1, .y1 = 0, .x2 = 1, .y2 = 5 },
		&ix, &iy
	);
	ck_assert_int_eq(found, 0);
	//~ ck_assert_int_eq(ix, 0);
	//~ ck_assert_int_eq(iy, 0);
	
	found = find_intersect(
		(ASeg) { .x1 = 0, .y1 = 1, .x2 = 5, .y2 = 1 },
		(ASeg) { .x1 = 4, .y1 = 1, .x2 = 9, .y2 = 1 },
		&ix, &iy
	);
	ck_assert_int_eq(found, 1);
	ck_assert_int_eq(ix, 4);
	ck_assert_int_eq(iy, 1);
	
	
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
	tcase_add_test(tc_bp, test_boilerplate_list);
	// not really testing memory allocations
	suite_add_tcase(s, tc_bp);
	
	
	/* Core test case */
	tc_core = tcase_create("Core");
	
	tcase_add_test(tc_core, test_parse_segment);
	tcase_add_test(tc_core, test_get_absolute_segment);
	tcase_add_test(tc_core, test_read_wire);
	tcase_add_test(tc_core, test_read_all_wires);
	tcase_add_test(tc_core, test_seg_length);
	tcase_add_test(tc_core, test_find_intersect);
	tcase_add_test(tc_core, test_find_intersect2);
	tcase_add_test(tc_core, test_compare_wires);
	tcase_add_test(tc_core, test_compare_wires2);
	tcase_add_test(tc_core, test_compare_wires3);
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
