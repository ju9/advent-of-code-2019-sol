// Task 2019/02A in C

#include <unistd.h> // read() and friends
#include <stdio.h> // file and io types and operations
#include <stdlib.h> // standard library definitions - memory allocations
// #include <string.h> // string and mem functions

// set file to stdin
int read_code(FILE *file)
{
	int code;
	int did_read;
	// read a number (code)
	//   and read and discard non-number (the ',' or '\n' separator)
	did_read = fscanf(file, "%d%*[^0123456789]", &code);
	
	if (did_read < 1) {
		return -1;
	}
	return code;
}

int upsize_tape(int **tape_ptr, int prev_size)
{
	int *new_tape;
	int size = (prev_size > 0)
		?	prev_size * 2
		:	32 // arbitrary starting size
	;
	
	new_tape = (int *) realloc(*tape_ptr, size * sizeof(int));
	
	if (new_tape == NULL) {
		return 0; // zero size; keep old tape for free()
	}
	
	*tape_ptr = new_tape;
	return size;
}

int load_tape(FILE *file, int **tape_ptr)
{
	int i = 0;
	int size = 0;
	int code;
	
	// load all the codes
	while ( ( code =/*=*/ read_code(file) ) != -1 ) {
		// if tape is too short, upsize the tape (realloc)
		if (size <= i) {
			size = upsize_tape(tape_ptr, size);
			// alloc fail
			if (size == 0) {
				return -1;
			}
		}
		
		// load the code onto tape
		(*tape_ptr)[i++] = code;
	}
	
	return 0;
}

void unload_tape(int *tape)
{
	free(tape);
}

// preset tape @1 and @2 with special values
void program_init(int *tape)
{
	tape[1] = 12;
	tape[2] = 2;
	return;
}


int get_indir(int *tape, int from)
{
	return tape[ // return value...
		tape[from] // ...referenced by value at 'from'
	];
}

void set_indir(int *tape, int from, int val)
{
	tape[ // set value...
		tape[from] // ...referenced by value at 'from'...
	] = val; // ...to 'val'
}

// process one operation starting at @PC; increment PC; return status "halted"
int computer_step(int *tape, int *pc)
{
	int halt = 0;
	int pc_increment = 1;
	
	switch (tape[*pc]) {
		case 1: // ADD
			set_indir(
				tape, *pc + 3,
				get_indir(tape, *pc + 1) + get_indir(tape, *pc + 2)
			);
			pc_increment += 3;
			break;
		case 2: // MULT
			set_indir(
				tape, *pc + 3,
				get_indir(tape, *pc + 1) * get_indir(tape, *pc + 2)
			);
			pc_increment += 3;
			break;
		case 99: // HALT
			halt = 1;
			break;
		default: // error
			halt = 1;
			break;
	}
	*pc += pc_increment;
	return halt;
}


// process operations until HALT
void computer_run(int *tape)
{
	int halt;
	int pc = 0;
	
	for (halt = 0; halt == 0; halt = computer_step(tape, &pc));

	return;
}
