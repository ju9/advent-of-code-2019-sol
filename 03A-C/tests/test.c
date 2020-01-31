// Task 2019/03A in C

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

START_TEST (test_intersect_distance)
{
	/* Distance from origin to the intersection of two segments
	*/
	ASeg seg1, seg2;
	int dist;
	
	#define ck_assert_aseg_is_dist(rx11, ry11, rx12, ry12, \
			rx21, ry21, rx22, ry22, ref) \
		seg1 = (ASeg) { .x1 = rx11, .y1 = ry11, .x2 = rx12, .y2 =  ry12 }; \
		seg2 = (ASeg) { .x1 = rx21, .y1 = ry21, .x2 = rx22, .y2 =  ry22 }; \
		dist = get_nearest_seg_intersect_dist(seg1, seg2); \
		ck_assert_int_eq(dist, ref);
	
	// crossing [3,4]
	ck_assert_aseg_is_dist(3,1,3,6, 1,4,5,4, 7);
	ck_assert_aseg_is_dist(1,4,5,4, 3,1,3,6, 7);
	
	// corner crossing
	ck_assert_aseg_is_dist(3,1,3,6, 3,1,5,1, 4);
	ck_assert_aseg_is_dist(3,1,3,6, 3,6,5,6, 9);
	ck_assert_aseg_is_dist(3,1,3,6, 1,1,3,1, 4);
	ck_assert_aseg_is_dist(3,1,3,6, 1,6,3,6, 9);
	
	// vert-overlap
	ck_assert_aseg_is_dist(7,1,7,6, 7,0,7,2, 8);
	ck_assert_aseg_is_dist(7,0,7,2, 7,1,7,6, 8);
	
	// horiz-overlap
	ck_assert_aseg_is_dist(2,5,8,5, 7,5,9,5, 12);
	ck_assert_aseg_is_dist(7,5,9,5, 2,5,8,5, 12);
	
	// misses
	ck_assert_aseg_is_dist(3,1,3,6, 4,7,8,7, 0);
	ck_assert_aseg_is_dist(4,7,8,7, 3,1,3,6, 0);
	ck_assert_aseg_is_dist(3,1,3,6, 4,0,4,2, 0);
	ck_assert_aseg_is_dist(3,0,3,2, 4,1,4,6, 0);
	ck_assert_aseg_is_dist(2,5,8,5, 7,6,9,6, 0);
	ck_assert_aseg_is_dist(7,5,9,5, 2,6,8,6, 0);
	
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
	
	int dist = get_nearest_intersect_dist(wire1, wire2);
	ck_assert_int_eq(dist, 6);
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
	
	int dist = get_nearest_intersect_dist(wire1, wire2);
	ck_assert_int_eq(dist, 159);
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
	
	int dist = get_nearest_intersect_dist(wire1, wire2);
	ck_assert_int_eq(dist, 135);
	list_drop(&wire1);
	list_drop(&wire2);
	
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
	tcase_add_test(tc_core, test_compare_wires);
	tcase_add_test(tc_core, test_compare_wires2);
	tcase_add_test(tc_core, test_compare_wires3);
	tcase_add_test(tc_core, test_intersect_distance);
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
