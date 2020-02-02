// Task 2019/03B in C

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

// get the lenght of the shortest loop:
//+ from origin [0,0] along wire1 to an intersection with wire2
//+ and along wire2 back to origin
int get_shortest_loop_length(Litem *wire1, Litem *wire2)
{
	Litem *current1, *current2;
	
	int min_length = 0;
	int length;
	int part_length_1, part_length_2;
	int ix, iy;
	
	current1 = wire1;
	part_length_1 = 0;
	while (current1) {
		current2 = wire2;
		part_length_2 = 0;
		while (current2) {
			if (find_intersect(current1->seg, current2->seg, &ix, &iy)) {
				length = part_length_1 + part_length_2
					+ get_length(current1->seg.x1, current1->seg.y1, ix, iy)
					+ get_length(current2->seg.x1, current2->seg.y1, ix, iy)
				;
				if (!min_length || length < min_length) {
					min_length = length;
				}
			}
			part_length_2 += get_seg_length(current2->seg);
			current2 = current2->next;
		}
		part_length_1 += get_seg_length(current1->seg);
		current1 = current1->next;
	}
	
	return min_length;
}

// return the length of segment given by the coordinates
int get_length(int x1, int y1, int x2, int y2)
{
	if (x1 == x2) {
		// vertical
		return abs(y1 - y2) + 1;
	}
	else {
		// horizontal; (non-ortho segments are not allowed)
		return abs(x1 - x2) + 1;
	}
}
int get_seg_length(ASeg seg)
{
	return get_length(seg.x1, seg.y1, seg.x2, seg.y2);
}

// find intersection between segments and return (bool) if  found;
//+ set referenced coordinates to the point of intersect
int find_intersect(ASeg seg1, ASeg seg2, int *ix, int *iy)
{
	// mode is bitmask for various orientations of segs
	int mode = (seg1.x1 == seg1.x2)
		+ (2 * (seg2.x1 == seg2.x2))
	; // 0 ~ HH, 1 ~ VH, 2 ~ HV, 3 ~ VV
	
	switch (mode) {
		case 0: // HH
			if (seg1.y1 != seg2.y1) return 0; // parallels miss
			*iy = seg1.y1;
			return find_collinear_intersect(seg1.x1, seg1.x2, seg2.x1, seg2.x2, ix);
			break;
		case 1: // VH
			return find_intersect_v_h(seg1, seg2, ix, iy);
			break;
		case 2: // HV
			// variation of the above case; same, just with rearranged params
			return find_intersect_v_h(seg2, seg1, ix, iy);
			break;
		case 3: // VV
		default:
			if (seg1.x1 != seg2.x1) return 0; // parallels miss
			*ix = seg1.x1;
			return find_collinear_intersect(seg1.y1, seg1.y2, seg2.y1, seg2.y2, iy);
			break;
	}	
}

int find_collinear_intersect(int a1, int a2, int b1, int b2, int *pos)
{
	int da = (a1 - a2);
	int db = (b1 - b2);
	if ((da * db) > 0) {
		// parallel
		// must find the intersect nearest a1 or b1
		if (da > 0) {
			// backwards
			if (a1 >= b1) {
				// b within or after a
				*pos = b1;
				return (b1 >= a2);
			}
			else {
				// a within or after b
				*pos = a1;
				return (a1 >= b2);
			}
		}
		else {
			// forwards
			if (a1 <= b1) {
				// b within or after a
				*pos = b1;
				return (b1 <= a2);
			}
			else {
				// a within or after b
				*pos = a1;
				return (a1 <= b2);
			}
		}
	}
	else {
		// counter-parallel
		// must find any intersect
		if (da > 0) {
			// a backwards; b forwards
			if (a1 >= b2) {
				// b's end may overlap a
				*pos = b2;
				return (b2 >= a2);
			}
			else {
				// a's start may overlap b
				*pos = a1;
				return (a1 >= b1);
			}
		}
		else {
			// a forwards; b backwards
			if (a1 <= b2) {
				// b's end may overlap a
				*pos = b2;
				return (b2 <= a2);
			}
			else {
				// a's start may overlap b
				*pos = a1;
				return (a1 <= b1);
			}
		}
	}
}

int find_intersect_v_h(ASeg segv, ASeg segh, int *ix, int *iy)
{
	if ( ((segv.x1 - segh.x1) * (segv.x1 - segh.x2)) > 0 ) {
		// bounding box miss on x
		return 0; 
	}
	if ( ((segh.y1 - segv.y1) * (segh.y1 - segv.y2)) > 0 ) {
		// bounding box miss on y
		return 0; 
	}
	*ix = segv.x1;
	*iy = segh.y1;
	return 1;
}
