#include "Cell.h"
#include "time_measuring.h"

#include <stdio.h>

#define FIXED_SIZE 100

Cell *top    = NULL;
Cell *bottom = NULL;

void append_fixed_top_prep(size_t n)
{
	// top = Cell_construct_list(FIXED_SIZE);
	bottom = Cell_construct_list(n);
	top = Cell_construct_list(FIXED_SIZE);
}

void append_fixed_bottom_prep(size_t n)
{
	top = Cell_construct_list(n);
	bottom = Cell_construct_list(FIXED_SIZE);
}

void append(size_t n)
{
	Cell_append(top, bottom);
}

void append_clean_up(size_t n)
{
	Cell_destruct_recursive(top);
}

size_t fixed_top_size_formula(size_t n)
{
	return n * 1000;
}

size_t fixed_bottom_size_formula(size_t n)
{
	return n * 1000;
}

void modify_measurements_divide_by_n(measurement_info *measurements)
{
	measurements->min /= (float)measurements->n;
	measurements->max /= (float)measurements->n;
	measurements->sum /= (float)measurements->n;
	measurements->avg /= (float)measurements->n;
}

int main(int argc, char *argv[])
{
	printf("%s %s %s\n", MAKE_CODENAME, MAKE_VERSION, MAKE_DATE);

	record_benchmark("plots/data/fixed_top.dat", append_fixed_top_prep, append, append_clean_up, NULL, fixed_top_size_formula, 100);

	record_benchmark("plots/data/fixed_bottom.dat", append_fixed_bottom_prep, append, append_clean_up, NULL, fixed_bottom_size_formula, 100);

	record_benchmark("plots/data/fixed_bottom_div_n.dat", append_fixed_bottom_prep, append, append_clean_up, modify_measurements_divide_by_n, fixed_bottom_size_formula, 100);

	return 0;
}
