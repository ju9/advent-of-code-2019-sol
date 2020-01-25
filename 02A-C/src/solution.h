// Task 2019/01A in C

#ifndef SOLUTION_H
#define SOLUTION_H

int read_code(FILE *file);

void program_init(int *tape);

int computer_step(int *tape, int *pc);

void computer_run(int *tape, int *pc);

#endif /* SOLUTION_H */