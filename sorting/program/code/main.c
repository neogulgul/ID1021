#include "ANSI.h"
#include "sorting.h"
#include "time_measuring.h"

#include <float.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static const size_t iterations_to_measure  = 100;
static const size_t iterations_to_find_min = 100;

int random_in_range(int min, int max)
{
	int range = max - min + 1;
	return rand() % range + min;
}

int *generate_random_array(size_t size)
{
	int *array = (int *)malloc(sizeof(int) * size);

	for (size_t i = 0; i < size; i++)
	{
		array[i] = random_in_range(0, RAND_MAX);
	}

	// // sorted front to back
	// for (size_t i = 0; i < size; i++)
	// {
	// 	array[i] = i;
	// }

	// // sorted back to front
	// for (size_t i = 0; i < size; i++)
	// {
	// 	array[i] = size - i - 1;
	// }

	return array;
}

void print_array(int *array, size_t size)
{
	printf("{ ");
	if (size > 0)
	{
		printf("%i", array[0]);
		for (size_t i = 1; i < size; i++)
		{
			printf(", %i", array[i]);
		}
	}
	printf(" }\n");
}

bool sorting_worked(int *sorted, int *original, size_t size)
{
	for (size_t i = 0; i < size - 1; i++)
	{
		if (sorted[i] > sorted[i + 1])
		{
			return false;
		}
	}

	size_t sorted_value_count = 0;
	for (size_t i = 0; i < size; i++)
	{
		int value = sorted[i];

		sorted_value_count++;

		if (i == size - 1 || sorted[i + 1] != value)
		{
			size_t original_value_count = 0;
			for (size_t j = 0; j < size; j++)
			{
				if (original[j] == value)
				{
					original_value_count++;
				}
			}
			if (original_value_count != sorted_value_count)
			{
				return false;
			}

			sorted_value_count = 0;
		}
	}

	return true;
}

void test_if_sort_works(void(*sort)(int *, size_t))
{
	size_t size = 10;

	int *array = generate_random_array(size);
	int *copy  = (int *)malloc(sizeof(int) * size);

	for (size_t i = 0; i < size; i++)
	{
		copy[i] = array[i];
	}

	printf("In  : ");
	print_array(array, size);
	sort(array, size);
	printf("Out : ");
	print_array(array, size);

	printf("Did sorting work? %s\n", sorting_worked(array, copy, size) ? ANSI_BOLD ANSI_GREEN "Ye :D" ANSI_OFF : ANSI_BOLD ANSI_RED "No :(" ANSI_OFF);

	free(array);
	free(copy);
}

void test_sorting()
{
	printf("Useless sort\n");
	test_if_sort_works(useless_sort);
	printf("Selection sort\n");
	test_if_sort_works(selection_sort);
	printf("Insertion sort\n");
	test_if_sort_works(insertion_sort);
	printf("Merge sort\n");
	test_if_sort_works(merge_sort);
}

typedef struct
{
	size_t array_size;
	float min;
	float max;
	float sum;
	float avg;
} measurement_info;

float measure_sort_algorithm(void(*sort)(int *, size_t), size_t array_size)
{
	int *array = generate_random_array(array_size);

	timespec t_start, t_stop;
	clock_gettime(CLOCK_MONOTONIC, &t_start);
	for (size_t i = 0; i < iterations_to_measure; i++)
	{
		sort(array, array_size);
	}
	clock_gettime(CLOCK_MONOTONIC, &t_stop);
	free(array);

	float time = nano_seconds(&t_start, &t_stop) / (float)iterations_to_measure;
	return time;
}

measurement_info measure_sort_algorithm_mulitple_times(void(*sort)(int *, size_t), size_t array_size)
{
	measurement_info measurements;

	measurements.array_size = array_size;
	measurements.min = FLT_MAX;
	measurements.max = 0;
	measurements.sum = 0;

	for (size_t i = 0; i < iterations_to_find_min; i++)
	{
		float time = measure_sort_algorithm(sort, array_size);
		measurements.sum += time;
		if (time < measurements.min) measurements.min = time;
		if (time > measurements.max) measurements.max = time;
	}

	measurements.avg = measurements.sum / iterations_to_find_min;

	return measurements;
}

void benchmark_sort(void(*sort)(int *, size_t), char *filepath, size_t different_sizes, size_t(*size_formula)(size_t), void(*modify_measurements)(measurement_info *))
{
	FILE *file = fopen(filepath, "w");
	printf("Opened \"%s\".\n", filepath);

	printf("             size          min          max          avg              sum\n");
	fprintf(file, "#     size          min          max          avg\n");

	for (size_t i = 0; i < different_sizes; i++)
	{
		size_t array_size = size_formula(i);

		measurement_info measurements = measure_sort_algorithm_mulitple_times(sort, array_size);

		if (modify_measurements != NULL)
		{
			modify_measurements(&measurements);
		}

		float progress = (float)(i + 1) / different_sizes * 100;

		printf("[%3.0f%%] %10u %12.2f %12.2f %12.2f %16.2f\n", progress, array_size, measurements.min, measurements.max, measurements.avg, measurements.sum);
		fprintf(file,    "%10u %12.2f %12.2f %12.2f\n",                  array_size, measurements.min, measurements.max, measurements.avg);
	}

	fclose(file);
	printf("Closed \"%s\".\n", filepath);
}

size_t size_formula_1(size_t i)
{
	return (i + 1) * 1;
}

size_t size_formula_2(size_t i)
{
	return (i + 1) * 10;
}

size_t size_formula_3(size_t i)
{
	return (i + 1) * 100;
}

size_t size_formula_4(size_t i)
{
	return (i + 1) * 1000;
}

void modify_measurements_1(measurement_info *measurements)
{
	measurements->min /= measurements->array_size;
	measurements->max /= measurements->array_size;
	measurements->avg /= measurements->array_size;
}

void modify_measurements_2(measurement_info *measurements)
{
	measurements->min /= log(measurements->array_size);
	measurements->max /= log(measurements->array_size);
	measurements->avg /= log(measurements->array_size);
}

int main(int argc, char *argv[])
{
	printf("%s %s %s\n", MAKE_CODENAME, MAKE_VERSION, MAKE_DATE);

	srand(time(NULL));

	// test_sorting();

	benchmark_sort(selection_sort, "plots/data/selection_sort.dat", 3, size_formula_14 NULL);

	// benchmark_sort(no_check_selection_sort, "plots/data/no_check_selection_sort.dat", 100, size_formula_1, NULL);

	benchmark_sort(insertion_sort, "plots/data/insertion_sort.dat", 10, size_formula_4, NULL);

	benchmark_sort(merge_sort, "plots/data/merge_sort.dat", 10, size_formula_4, NULL);

	// benchmark_sort(merge_sort, "plots/data/merge_sort_division_by_n.dat", 100, size_formula_3, modify_measurements_1);

	// benchmark_sort(merge_sort, "plots/data/merge_sort_division_by_log_n.dat", 100, size_formula_3, modify_measurements_2);

	return 0;
}
