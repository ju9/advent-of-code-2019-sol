// Task 2019/02A in C

#include <unistd.h> // read() and friends
#include <stdio.h> // file and io types and operations
#include <stdlib.h> // standard library definitions - memory allocations
#include <string.h> // string and mem functions

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

// load input into memory (tape)
// return: size (item count) | -1 (error)
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
	
	return i;
}

void unload_tape(int *tape)
{
	free(tape);
}

int *init_working_tape(int *master_tape, int item_count)
{
	int *working_tape;
	
	if (master_tape == NULL || item_count < 1) return NULL;
	
	working_tape = malloc(item_count * sizeof(int));
	if (working_tape == NULL) return NULL;
	
	return working_tape;
}

void preset_working_tape(int *working_tape, int *master_tape, int item_count)
{
	memcpy(working_tape, master_tape, item_count * sizeof(int));
}

// preset tape @1 and @2 with special values
void program_init(int *tape, int noun, int verb)
{
	tape[1] = noun;
	tape[2] = verb;
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

// the solution's format
int encode_params(int noun, int verb)
{
	return (100 * noun) + verb;
}

// search thru noun-verb combinations until found one that yields 'target'
int run_search(int target, int *working_tape, int *master_tape, int item_count)
{
	int noun, verb;
	
	for (noun = 0; noun < 100; noun++) {
		for (verb = 0; verb < 100; verb++) {
			// preset working tape the same as master_tape
			preset_working_tape(working_tape, master_tape, item_count);
			
			// set program parameters
			program_init(working_tape, noun, verb);
			
			// run until HALT
			computer_run(working_tape);
			
			// compare result with target:
			//+ if matching, report the generating parameters
			if (working_tape[0] == target) {
				return encode_params(noun, verb);
			}
			
		}
	}
	
	return -1;
}
