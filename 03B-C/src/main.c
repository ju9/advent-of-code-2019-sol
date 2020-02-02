// Task 2019/03B in C

#include <stdio.h> // file and io types and operations
#include <stdlib.h> // standard library definitions - memory allocations
#include "./solution.h"

int main(void)
{
	Litem *wire1 = read_wire(stdin);
	Litem *wire2 = read_wire(stdin);
	
	int length = get_shortest_loop_length(wire1, wire2);
	
	list_drop(&wire1);
	list_drop(&wire2);

	printf("%d\n", length);
}