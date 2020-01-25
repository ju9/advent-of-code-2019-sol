// Task 2019/02A in C

#include <stdio.h> // file and io types and operations
#include <stdlib.h> // standard library definitions - memory allocations
#include "./solution.h"

int main(void)
{
	int *tape = NULL;
	int err;
	
	// read std-input onto tape, (initialize tape)
	err = load_tape(stdin, &tape);

	if (err == 0) {
		// initialize some values on the tape
		program_init(tape);
		
		// process the codes on tape
		computer_run(tape);
		
		// return @0
		printf("%d\n", tape[0]);
	}
	
	// free tape resources
	unload_tape(tape);
	
	return err;
	
}