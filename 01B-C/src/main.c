// Task 2019/01B in C

#include <stdio.h> // file and io types and operations
#include <stdlib.h> // standard library definitions - memory allocations
#include "./solution.h"

int main(void)
{
	int fuel_req = read_evaluate_all_modules(stdin);
	printf("%d\n", fuel_req);
}