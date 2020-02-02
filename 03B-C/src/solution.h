// Task 2019/03B in C

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

int get_shortest_loop_length(Litem *wire1, Litem *wire2);

int get_length(int x1, int y1, int x2, int y2);
int get_seg_length(ASeg seg);

int find_intersect(ASeg seg1, ASeg seg2, int *ix, int *iy);

int find_collinear_intersect(int a1, int a2, int b1, int b2, int *pos);

int find_intersect_v_h(ASeg seg1, ASeg seg2, int *ix, int *iy);

#endif /* SOLUTION_H */