// Task 2019/01A in C

#include <unistd.h> // read() and friends
#include <stdio.h> // file and io types and operations
#include <stdlib.h> // standard library definitions - memory allocations
// #include <string.h> // string and mem functions

// set file to stdin
int read_code(FILE *file)
{
	int mass;
	int did_read;
	// read a number (mass)
	//   and read and discard non-number (the ',' or '\n' separator)
	did_read = fscanf(file, "%d%*[^0123456789]", &mass);
	
	if (did_read < 1) {
		return -1;
	}
	return mass;
}

// preset tape @1 and @2 with special values
void program_init(int *tape)
{
	// stub
	return;
}

// process one operation starting at @PC; increment PC; return status "halted"
int computer_step(int *tape, int *pc)
{
	// stub
	return -1;
}

// process operations until HALT
void computer_run(int *tape, int *pc)
{
	int halted = 0;
	
	// stub
	return;
}
