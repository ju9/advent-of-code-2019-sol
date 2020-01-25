// Task 2019/02A in C

#include <stdio.h> // file and io types and operations
#include <stdlib.h> // standard library definitions - memory allocations
#include "./solution.h"

int main(void)
{
	int *master_tape = NULL;
	int *working_tape = NULL;
	int item_count;
	int err = 0;
	int target = 19690720;
	int answer = -1;
	
	// read std-input onto tape, (initialize tape)
	item_count = load_tape(stdin, &master_tape);
	if (item_count < 0) err = -1;
	
	if (!err) {
		working_tape = init_working_tape(master_tape, item_count);
		if (working_tape == NULL) err = -2;
	}
	
	if (!err) {
		// search for target
		answer = run_search(target, working_tape, master_tape, item_count);
		printf("%d\n", answer);
		
		if (answer == -1) err = 1; // whatever
	}
	
	// free tape resources
	unload_tape(master_tape);
	unload_tape(working_tape);
	
	return err;
	
}