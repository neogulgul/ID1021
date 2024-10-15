#include "testing.h"

#include "Area.h"
#include "time_measuring.h"

#include <stdio.h>
#include <stdlib.h>

static csv_info info = { NULL, 0, sizeof(Area) };

static void function_prep(size_t n)
{
}

static void function(size_t n)
{
}

static void function_clean_up(size_t n)
{
}

static size_t smallest_number_of_collisions   = -1;
static size_t smallest_number_of_collisions_n =  0;

static void modify_measurements(measurement_info *measurements)
{
	size_t n = measurements->n;

	size_t collisions[n];
	for (size_t i = 0; i < n; i++)
	{
		collisions[i] = 0;
	}

	for (size_t i = 0; i < info.array_size; i++)
	{
		Area area = ((Area *)info.array)[i];
#ifdef ZIP_CODE_INTEGER
		collisions[area.zip_code % n]++;
#else
		collisions[zip_code_str_to_int(area.zip_code) % n]++;
#endif
	}

	measurements->min = 0;
	for (size_t i = 0; i < n; i++)
	{
		if (collisions[i] > 1)
		{
			measurements->min += collisions[i] - 1;
		}
	}

	if (measurements->min < smallest_number_of_collisions)
	{
		smallest_number_of_collisions   = measurements->min;
		smallest_number_of_collisions_n = n;
	}
}

static size_t size_formula(size_t n)
{
	return n * 200;
}

void testing_b()
{
	if (csv_info_read_from_file(&info, "postnummer.csv", Area_process_line))
	{
		record_benchmark("plots/data/collisions.dat", function_prep, function, function_clean_up, modify_measurements, size_formula, 100);

		printf("Specific modulo values:\n");
		printf("\tmod 10000 has %li collisions.\n", (long)measure_function_mulitple_times(function_prep, function, function_clean_up, modify_measurements, 10000).min);
		printf("\tmod 20000 has %li collisions.\n", (long)measure_function_mulitple_times(function_prep, function, function_clean_up, modify_measurements, 20000).min);
		printf("\tmod 12345 has %li collisions.\n", (long)measure_function_mulitple_times(function_prep, function, function_clean_up, modify_measurements, 12345).min);
		printf("\tmod 17389 has %li collisions.\n", (long)measure_function_mulitple_times(function_prep, function, function_clean_up, modify_measurements, 17389).min);
		printf("\tmod 13513 has %li collisions.\n", (long)measure_function_mulitple_times(function_prep, function, function_clean_up, modify_measurements, 13513).min);
		printf("\tmod 13600 has %li collisions.\n", (long)measure_function_mulitple_times(function_prep, function, function_clean_up, modify_measurements, 13600).min);
		printf("\tmod 14000 has %li collisions.\n", (long)measure_function_mulitple_times(function_prep, function, function_clean_up, modify_measurements, 14000).min);

		// finding the smallest number of collisions
		for (size_t i = 1; i <= 20000; i++)
		{
			measure_function_mulitple_times(function_prep, function, function_clean_up, modify_measurements, i);
		}

		printf("mod %lu had the smallest number of collisions: %lu\n", smallest_number_of_collisions_n, smallest_number_of_collisions);

		csv_info_free(&info, Area_pointer_free);
	}
}
