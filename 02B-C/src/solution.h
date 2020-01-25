// Task 2019/02A in C

#ifndef SOLUTION_H
#define SOLUTION_H

int read_code(FILE *file);

int upsize_tape(int **tape_ptr, int prev_size);

int load_tape(FILE *file, int **tape_ptr);

void unload_tape(int *tape);

int *init_working_tape(int *master_tape, int item_count);

void preset_working_tape(int *working_tape, int *master_tape, int item_count);

void program_init(int *tape, int noun, int verb);

int get_indir(int *tape, int from);

void set_indir(int *tape, int from, int val);

int computer_step(int *tape, int *pc);

void computer_run(int *tape);

int encode_params(int noun, int verb);

int run_search(int target, int *working_tape, int *master_tape, int item_count);

#endif /* SOLUTION_H */