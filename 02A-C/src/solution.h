// Task 2019/02A in C

#ifndef SOLUTION_H
#define SOLUTION_H

int read_code(FILE *file);

int upsize_tape(int **tape_ptr, int prev_size);

int load_tape(FILE *file, int **tape_ptr);

void unload_tape(int *tape);

void program_init(int *tape);

int get_indir(int *tape, int from);

void set_indir(int *tape, int from, int val);

int computer_step(int *tape, int *pc);

void computer_run(int *tape, int *pc);

#endif /* SOLUTION_H */