#include "time_measuring.h"

#include <float.h>
#include <stdio.h>

size_t iterations_to_find_min = 100;

long nano_seconds(timespec *t_start, timespec *t_stop)
{
	return (t_stop->tv_nsec - t_start->tv_nsec) +
	       (t_stop->tv_sec - t_start->tv_sec) * NANOSECONDS_PER_SECOND;
}

float measure_function(void (*function_prep)(size_t), void (*function)(size_t), void (*function_clean_up)(size_t), size_t n)
{
	timespec t_start, t_stop;

	if (function_prep != NULL) function_prep(n);

	clock_gettime(CLOCK_MONOTONIC, &t_start);

	if (function != NULL) function(n);

	clock_gettime(CLOCK_MONOTONIC, &t_stop);

	if (function_clean_up != NULL) function_clean_up(n);

	return (float)nano_seconds(&t_start, &t_stop);
}

measurement_info measure_function_mulitple_times(void (*function_prep)(size_t), void (*function)(size_t), void (*function_clean_up)(size_t), size_t n)
{
	measurement_info measurements;

	measurements.n = n;
	measurements.min = FLT_MAX;
	measurements.max = 0;
	measurements.sum = 0;

	for (size_t i = 0; i < iterations_to_find_min; i++)
	{
		float time = measure_function(function_prep, function, function_clean_up, n);
		measurements.sum += time;
		if (time < measurements.min) measurements.min = time;
		if (time > measurements.max) measurements.max = time;
	}

	measurements.avg = measurements.sum / iterations_to_find_min;

	return measurements;
}

void record_benchmark(
	char    *filepath,
	void   (*function_prep)(size_t),
	void   (*function)(size_t),
	void   (*function_clean_up)(size_t),
	void   (*modify_measurements)(measurement_info *),
	size_t (*size_formula)(size_t),
	size_t   different_sizes
)
{
	FILE *file = fopen(filepath, "w");
	printf("Opened \"%s\".\n", filepath);

	printf("             size          min          max          avg              sum\n");
	fprintf(file, "#     size          min          max          avg\n");

	for (size_t i = 1; i <= different_sizes; i++)
	{
		size_t n = size_formula == NULL ? i : size_formula(i);

		measurement_info measurements = measure_function_mulitple_times(function_prep, function, function_clean_up, n);

		if (modify_measurements != NULL)
		{
			modify_measurements(&measurements);
		}

		float progress = (float)i / different_sizes * 100.f;

		printf("[%3.0f%%] %10u %12.2f %12.2f %12.2f %16.2f\n", progress, n, measurements.min, measurements.max, measurements.avg, measurements.sum);
		fprintf(file,    "%10u %12.2f %12.2f %12.2f\n",                  n, measurements.min, measurements.max, measurements.avg);
	}

	fclose(file);
	printf("Closed \"%s\".\n", filepath);
}
