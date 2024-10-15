#pragma once

#include <time.h>

#define NANOSECONDS_PER_SECOND 1000000000

typedef struct
{
	size_t n;
	float min;
	float max;
	float sum;
	float avg;
} measurement_info;

typedef struct timespec timespec;

long nano_seconds(timespec *t_start, timespec *t_stop);

measurement_info measure_function_mulitple_times(
	void (*function_prep)(size_t),
	void (*function)(size_t),
	void (*function_clean_up)(size_t),
	void (*modify_measurements)(measurement_info *),
	size_t n
);

void record_benchmark(
	char    *filepath,
	void   (*function_prep)(size_t),
	void   (*function)(size_t),
	void   (*function_clean_up)(size_t),
	void   (*modify_measurements)(measurement_info *),
	size_t (*size_formula)(size_t),
	size_t   different_sizes
);
