// Task 2019/03A in C

#include <unistd.h> // read() and friends
#include <stdio.h> // file and io types and operations
#include <stdlib.h> // standard library definitions - memory allocations
// #include <string.h> // string and mem functions
#include "solution.h"


// Singly linked lists

// create a new list (new list tail node)
Litem *list_new(ASeg payload)
{
	Litem *new_list = malloc(sizeof(Litem));
	new_list->next = NULL;
	//~ new_list->seg = (ASeg) { .x1 = 0, .y1 = 0, .x2 = 0, .y2 = 0 };
	new_list->seg = payload;
	return new_list;
}

// deallocate the whole list
void list_drop(Litem **list_ptr)
{
	list_destroy(*list_ptr);
	*list_ptr = NULL;
}

// helper to list_drop
void list_destroy(Litem *list)
{
	if (list->next != NULL) {
		list_destroy(list->next);
	}
	free(list);
}


// IO

// read one code entry from input file into code_buffer; return status
// set file to stdin
int read_segment_code(FILE *file, char *code_buffer)
{
	char sep = 'x';
	int did_read;
	// read a segment code and a following separator
	did_read = fscanf(file, "%[^\n,]%c", code_buffer, &sep);
	
	if (did_read == 1) {
		return READ_EOF;
	}
	else if (did_read == 0) {
		return READ_ERR;
	}
	else if (sep == ',') {
		return READ_OK;
	}
	else if (sep == '\n') {
		return READ_EOL;
	}
	return READ_ERR;
}

// data handling

// parse segment code into relative segment struct
RSeg parse_segment(char *seg_code)
{
	int len;
	char dir;
	int status = sscanf(seg_code, "%c%d", &dir, &len);
	if (
		(status < 2) ||
		(dir != DIR_UP && dir != DIR_DOWN && dir != DIR_LEFT && dir != DIR_RIGHT)
	) {
		dir = DIR_INVALID;
		len = 0;
	}
	
	return (RSeg) { .len = len, .dir = dir };
}

// from relative segment (direction, length), from given origin, derive
//+ an absolute segment (coordinates of start & end);
//+ exclude the starting point (overlaps the end of preceding segment)
ASeg get_absolute_segment(RSeg rseg, int orig_x, int orig_y)
{
	ASeg aseg = { .x1 = orig_x, .y1 = orig_y, .x2 = orig_x, .y2 = orig_y };
	int skip_len = rseg.len ? 1 : 0;
	
	switch (rseg.dir) {
		case DIR_UP:
			aseg.y2 -= rseg.len; // ending point
			aseg.y1 -= skip_len; // step over starting point
			break;
		case DIR_DOWN:
			aseg.y2 += rseg.len;
			aseg.y1 += skip_len;
			break;
		case DIR_LEFT:
			aseg.x2 -= rseg.len;
			aseg.x1 -= skip_len;
			break;
		case DIR_RIGHT:
			aseg.x2 += rseg.len;
			aseg.x1 += skip_len;
			break;
		default:
			break;
	}
	return aseg;
}

// process input stream into list of absolute segments
Litem *read_wire(FILE *file)
{
	char code_buffer[8];
	int status;
	Litem root = {
		.next = NULL,
		.seg = (ASeg) { .x1 = 0, .y1 = 0, .x2 = 0, .y2 = 0 }
	};
	
	Litem *current = &root;
	
	while (1) {
		status =/*=*/ read_segment_code(file, code_buffer);
		if (status == READ_ERR) {
			break;
		}
		
		current->next = list_new(
			get_absolute_segment(
				parse_segment(code_buffer),
				current->seg.x2,
				current->seg.y2
			)
		);
		current = current->next;
		
		if (status != READ_OK) {
			break;
		}
	}
	
	return root.next;
}

// get the nearest (to origin [0,0]) intersection of two wires
int get_nearest_intersect_dist(Litem *wire1, Litem *wire2)
{
	Litem *current1 = wire1;
	Litem *current2;
	
	int min_dist = 0;
	int dist;
	
	
	while (current1) {
		current2 = wire2;
		while (current2) {
			dist = get_nearest_seg_intersect_dist(current1->seg, current2->seg);
			if (dist && (!min_dist || dist < min_dist)) {
				min_dist = dist;
			}
			current2 = current2->next;
		}
		current1 = current1->next;
	}
	
	return min_dist;
}

// get distance from origin [0,0] to intersection of two segments,
//+ or 0 if not intersecting
int get_nearest_seg_intersect_dist(ASeg seg1, ASeg seg2)
{
	// mode is bitmask for various orientations of segs
	int mode = (seg1.x1 == seg1.x2)
		+ (2 * (seg2.x1 == seg2.x2))
	; // 0 ~ HH, 1 ~ VH, 2 ~ HV, 3 ~ VV
	
	int tmp = 0;
	
	switch (mode) {
		case 0: // HH
			if (seg1.y1 != seg2.y1) return 0; // parallels miss
			if (!find_nearest_collinear_intersect_pos(
					seg1.x1, seg1.x2, seg2.x1, seg2.x2, &tmp)
			) return 0; // no overlap
			return get_origin_dist(tmp, seg1.y1);
			break;
		case 1: // VH
			return get_intersect_dist_v_h(seg1, seg2);
			break;
		case 2: // HV
			// variation of the above case; same, just with rearranged params
			return get_intersect_dist_v_h(seg2, seg1);
			break;
		case 3: // VV
		default:
			if (seg1.x1 != seg2.x1) return 0; // parallels miss
			if (!find_nearest_collinear_intersect_pos(
					seg1.y1, seg1.y2, seg2.y1, seg2.y2, &tmp)
			) return 0; // no overlap
			return get_origin_dist(seg1.x1, tmp);
			break;
	}
	
}

// helper function
// get distance from origin [0,0] to intersection of two perpendicular segments,
//+ or 0 if not intersecting
int get_intersect_dist_v_h(ASeg segv, ASeg segh)
{
	if ( ((segv.x1 - segh.x1) * (segv.x1 - segh.x2)) > 0 ) {
		// bounding box miss on x
		return 0; 
	}
	if ( ((segh.y1 - segv.y1) * (segh.y1 - segv.y2)) > 0 ) {
		// bounding box miss on y
		return 0; 
	}
	return get_origin_dist(segv.x1, segh.y1);
}

// helper function
// look at two collinear segments;
//+ set ref-result: nearest overlap position;
//+ return (bool) if overlapping
int find_nearest_collinear_intersect_pos(int a1, int a2, int b1, int b2, int *pos)
{
	// reorder the values to ascending
	int tmp;
	if (a1 > a2) { tmp = a1; a1 = a2; a2 = tmp; }
	if (b1 > b2) { tmp = b1; b1 = b2; b2 = tmp; }
	
	if (a2 < b1 || b2 < a1) {
		// no overlap
		return 0;
	}
	
	if (a1 <= b1) {
		if (a2 < b2) {
			// partial overlap; a before b
			*pos = get_nearest_pos(b1, a2);
		}
		else {
			// b fully contained in a
			*pos = get_nearest_pos(b1, b2);
		}
		
	} else { // b1 > a1
		if (b2 < a2) {
			// partial overlap; b before a
			*pos = get_nearest_pos(a1, b2);
		}
		else {
			// a fully contained in b
			*pos = get_nearest_pos(a1, a2);
		}
	}
	return 1;
	
}

// helper function
int get_nearest_pos(int a, int b)
{
	if (a * b < 0) {
		return 0; // overlap at 0
	}
	if (abs(a) <= abs(b)) {
		return a;
	}
	else {
		return b;
	}
}

// manhattan distance
int get_origin_dist(int x, int y)
{
	return abs(x) + abs(y);
}
