#include "search_algorithms.h"
#include "time_measuring.h"
#include "generate_array.h"

#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void print_int_array(int *array, size_t array_size)
{
	printf("{ ");
	if (array_size > 0)
	{
		printf("%i", array[0]);
		for (size_t i = 1; i < array_size; i++)
		{
			printf(", %i", array[i]);
		}
	}
	printf(" }\n");
}

float measure_search_algorithm(bool(*search_algorithm)(int *, size_t, int), size_t array_size, size_t key_searches, ARRAY_TYPE array_type)
{
	int *array = generate_array(array_size, 0, RAND_MAX, array_type);
	int *keys = generate_array(key_searches, 0, RAND_MAX, ARRAY_TYPE_UNSORTED);

	timespec t_start, t_stop;
	clock_gettime(CLOCK_MONOTONIC, &t_start);
	for (size_t i = 0; i < key_searches; i++)
	{
		search_algorithm(array, array_size, keys[i]);
	}
	clock_gettime(CLOCK_MONOTONIC, &t_stop);

	free(array);
	free(keys);

	float time = nano_seconds(&t_start, &t_stop) / (float)key_searches;
	return time;
}

typedef struct
{
	float min;
	float max;
	float sum;
	float avg;
	float ratio;
} measurement_info;

measurement_info measure_search_algorithm_multiple_times(
	bool(*search_algorithm)(int *, size_t, int),
	size_t array_size,
	size_t key_searches,
	ARRAY_TYPE array_type,
	size_t iterations_to_find_min,
	bool print_stuff
)
{
	float min = FLT_MAX;
	float max = 0;
	float sum = 0;

	for (size_t i = 0; i < iterations_to_find_min; i++)
	{
		float time = measure_search_algorithm(search_algorithm, array_size, key_searches, array_type);
		sum += time;
		if (time < min) min = time;
		if (time > max) max = time;

		if (print_stuff)
		{
			float progress = (i + 1) / (float)iterations_to_find_min * 100;
			printf("[%3.0f%%]\n", progress);
		}
	}

	float avg = sum / iterations_to_find_min;
	float ratio = array_size / min;

	return (measurement_info){ min, max, sum, avg, ratio };
}

typedef struct
{
	ARRAY_TYPE array_type;
	size_t different_array_sizes;
	size_t array_size_multiplier;
	size_t iterations_to_find_min;
	size_t key_searches;
} benchmark_settings;

void benchmark_search_algorithm(char *filepath, bool(*search_algorithm)(int *, size_t, int), benchmark_settings settings)
{
	FILE *file = fopen(filepath, "w");
	printf("Opened \"%s\".\n", filepath);

	printf("settings.different_array_sizes  : %u\n", settings.different_array_sizes );
	printf("settings.array_size_multiplier  : %u\n", settings.array_size_multiplier );
	printf("settings.iterations_to_find_min : %u\n", settings.iterations_to_find_min);
	printf("settings.key_searches           : %u\n", settings.key_searches          );

	printf("             size        min        max        avg      ratio              sum\n");
	fprintf(file, "# ratio = size / min\n");
	fprintf(file, "#     size        min        max        avg      ratio\n");

	size_t key_searches = settings.key_searches;

	for (size_t i = 0; i <= settings.different_array_sizes; i++)
	{
		size_t array_size = i == 0 ? 1 : i * settings.array_size_multiplier;

		measurement_info measurements = measure_search_algorithm_multiple_times(
			search_algorithm,
			array_size,
			key_searches,
			settings.array_type,
			settings.iterations_to_find_min,
			false
		);

		float progress = (float)i / settings.different_array_sizes * 100;

		printf("[%3.0f%%] %10u %10.2f %10.2f %10.2f %10.2f %16.2f\n", progress, array_size, measurements.min, measurements.max, measurements.avg, measurements.ratio, measurements.sum);
		fprintf(file, "%10u %10.2f %10.2f %10.2f %10.2f\n", array_size, measurements.min, measurements.max, measurements.avg, measurements.ratio);
	}

	fclose(file);
	printf("Closed \"%s\".\n", filepath);
}

void binary_search_64M()
{
	printf(
		"-----------------\n"
		"binary_search_64M\n"
		"-----------------\n"
	);
	printf("Please be patient. This may take some time :)\n");
	measurement_info info = measure_search_algorithm_multiple_times(binary_search, 64000000, 100, ARRAY_TYPE_SORTED, 30, true);
	printf("measurement_info.min   : %f\n", info.min  );
	printf("measurement_info.max   : %f\n", info.max  );
	printf("measurement_info.sum   : %f\n", info.sum  );
	printf("measurement_info.avg   : %f\n", info.avg  );
	printf("measurement_info.ratio : %f\n", info.ratio);

	/*
		just keeping the values i got here since i am lazy
		measurement_info.min   : 570.770020
		measurement_info.max   : 1314.880005
		measurement_info.sum   : 19133.843750
		measurement_info.avg   : 637.794800
		measurement_info.ratio : 112129.226562
	*/
}

int main(int argc, char *argv[])
{
	printf("%s %s %s\n", MAKE_CODENAME, MAKE_VERSION, MAKE_DATE);

	srand(time(NULL));

	// binary_search_64M();
	// return 0;

	bool bench_unsorted_brute_search   = 1;
	bool bench_sorted_brute_search     = 1;
	bool bench_binary_search           = 1;
	bool bench_recursive_binary_search = 1;

	if (bench_unsorted_brute_search)
	{
		benchmark_search_algorithm(
			"plots/data/plot_brute_unsorted.dat",
			unsorted_brute_search,
			(benchmark_settings){
				ARRAY_TYPE_UNSORTED,
				10,
				100000,
				30,
				100
			}
		);
	}

	if (bench_sorted_brute_search)
	{
		benchmark_search_algorithm(
			"plots/data/plot_brute_sorted.dat",
			sorted_brute_search,
			(benchmark_settings){
				ARRAY_TYPE_SORTED,
				10,
				100000,
				30,
				100
			}
		);

		benchmark_search_algorithm(
			"plots/data/plot_brute_sorted_simple.dat",
			sorted_brute_search,
			(benchmark_settings){
				ARRAY_TYPE_SORTED_SIMPLE,
				10,
				100000,
				30,
				100
			}
		);
	}

	if (bench_binary_search)
	{
		benchmark_search_algorithm(
			"plots/data/plot_binary.dat",
			binary_search,
			(benchmark_settings){
				ARRAY_TYPE_SORTED,
				100,
				10000,
				30,
				100
			}
		);
	}

	if (bench_recursive_binary_search)
	{
		benchmark_search_algorithm(
			"plots/data/plot_binary_recursive.dat",
			recursive_binary_search,
			(benchmark_settings){
				ARRAY_TYPE_SORTED,
				100,
				10000,
				30,
				100
			}
		);
	}

	return 0;
}
