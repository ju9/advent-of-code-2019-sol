// Task 2019/03A in C

#ifndef SOLUTION_H
#define SOLUTION_H



// Ortho line segment

enum direction {
	DIR_UP = 'U',
	DIR_DOWN = 'D',
	DIR_LEFT = 'L',
	DIR_RIGHT = 'R',
	DIR_INVALID = 'X',
};

// - relative notation
typedef struct RSeg {
	int len;
	enum direction dir;
} RSeg;

// - absolute notation
typedef struct ASeg {
	int x1;
	int y1;
	int x2;
	int y2;
} ASeg;

// Singly linked list

typedef struct Litem {
	ASeg seg;
	struct Litem *next;
} Litem;


#define READ_OK 0
#define READ_EOL -1
#define READ_EOF -2
#define READ_ERR -3

Litem *list_new(ASeg payload);

void list_drop(Litem **list_ptr);

void list_destroy(Litem *list);

int read_segment_code(FILE *file, char *code_buffer);

RSeg parse_segment(char *seg_code);

ASeg get_absolute_segment(RSeg rseg, int orig_x, int orig_y);

Litem *read_wire(FILE *file);

int get_nearest_intersect_dist(Litem *wire1, Litem *wire2);

int get_nearest_seg_intersect_dist(ASeg seg1, ASeg seg2);

	int get_intersect_dist_v_h(ASeg segv, ASeg segh);
	
	int find_nearest_collinear_intersect_pos(int a1, int a2, int b1, int b2, int *pos);
	
	int get_nearest_pos(int a, int b);
	
int get_origin_dist(int x, int y);

#endif /* SOLUTION_H */