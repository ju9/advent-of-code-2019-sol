// Task 2019/01A in C

#ifndef SOLUTION_H
#define SOLUTION_H

int get_module_fuel_req(int mass);

void account_fuel_req(int *fuel_req, int module_fuel_req);

int read_module_mass(FILE *file);

int read_evaluate_all_modules(FILE *file);

#endif /* SOLUTION_H */