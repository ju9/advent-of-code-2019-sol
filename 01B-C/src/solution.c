// Task 2019/01B in C

#include <unistd.h> // read() and friends
#include <stdio.h> // file and io types and operations
#include <stdlib.h> // standard library definitions - memory allocations
// #include <string.h> // string and mem functions

int get_module_fuel_req(int mass)
{
	int fuel_req = (mass / 3) - 2;
	if (fuel_req > 0) {
		// get also fuel requirement of fuel mass
		fuel_req += get_module_fuel_req(fuel_req);
	} else {
		// clamp negative values to zero
		fuel_req = 0;
	}
	return fuel_req;
}

void account_fuel_req(int *fuel_req, int module_fuel_req)
{
	*fuel_req += module_fuel_req;
}


// set file to stdin
int read_module_mass(FILE *file)
{
	int mass;
	int did_read;
	// read a number (mass)
	//   and read and discard non-number (the ',' separator)
	did_read = fscanf(file, "%d%*[^0123456789]", &mass);
	
	if (did_read < 1) {
		return -1;
	}
	return mass;
}



// set file to stdin
int read_evaluate_all_modules(FILE *file)
{
	int mass;
	int fuel_req = 0;
	
	while ( ( mass =/*=*/ read_module_mass(file) ) != -1 ) {
		account_fuel_req(&fuel_req,
			get_module_fuel_req(mass)
		);
	}

	return fuel_req;
}







