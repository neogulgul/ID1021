#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NANOSECONDS_PER_SECOND 1000000000

long nano_seconds(struct timespec *t_start, struct timespec *t_stop)
{
	return (t_stop->tv_nsec - t_start->tv_nsec) +
	       (t_stop->tv_sec - t_start->tv_sec) * NANOSECONDS_PER_SECOND;
}

long bench_sequential_access(int n, int loops)
{
	// Empty arrays are not allowed!
	if (n <= 0) return 0;

	struct timespec t_start, t_stop;

	int *array = (int *)malloc(n * sizeof(int));
	// We do not want any element to be zero.
	// That would run the risk of the sum being zero.
	for (int i = 0; i < n; i++) array[i] = i + 1;

	int sum = 0;

	clock_gettime(CLOCK_MONOTONIC, &t_start);
	for (int i = 0; i < loops; i++) sum += array[i % n];
	clock_gettime(CLOCK_MONOTONIC, &t_stop);

	// Avoiding memory leaks.
	free(array);

	// Avoiding compiler optimizations that would give misleading measurements.
	if (sum == 0) return 0;

	return nano_seconds(&t_start, &t_stop);
}

long bench_random_access(int n, int loops)
{
	// Empty arrays are not allowed!
	if (n <= 0) return 0;

	struct timespec t_start, t_stop;

	int *array = (int *)malloc(n * sizeof(int));
	// We do not want any element to be zero.
	// That would run the risk of the sum being zero.
	for (int i = 0; i < n; i++) array[i] = i + 1;

	int *indices = (int *)malloc(loops * sizeof(int));
	for (int i = 0; i < loops; i++) indices[i] = rand() % n;

	int sum = 0;

	clock_gettime(CLOCK_MONOTONIC, &t_start);
	for (int i = 0; i < loops; i++) sum += array[indices[i]];
	clock_gettime(CLOCK_MONOTONIC, &t_stop);

	// Avoiding memory leaks.
	free(array);
	free(indices);

	// Avoiding compiler optimizations that would give misleading measurements.
	if (sum == 0) return 0;

	// Dividing by 2 since we are performing 2 array accesses.
	return nano_seconds(&t_start, &t_stop) / 2;
}

long bench_search(int n, int loops)
{
	// Empty arrays are not allowed!
	if (n <= 0) return 0;

	struct timespec t_start, t_stop;

	int *array = (int *)malloc(n * sizeof(int));
	for (int i = 0; i < n; i++) array[i] = rand() % (n * 2);

	int *keys = (int *)malloc(loops * sizeof(int));
	for (int i = 0; i < loops; i++) keys[i] = rand() % (n * 2);

	int sum = 0;

	clock_gettime(CLOCK_MONOTONIC, &t_start);
	for (int i = 0; i < loops; i++)
	{
		int key = keys[i];
		for (int j = 0; j < n; j++)
		{
			if (key == array[j])
			{
				sum++;
				break;
			}
		}
	}
	clock_gettime(CLOCK_MONOTONIC, &t_stop);

	// Avoiding memory leaks.
	free(array);
	free(keys);

	// Avoiding compiler optimizations that would give misleading measurements.
	if (sum == 0) return 0;

	return nano_seconds(&t_start, &t_stop);
}

long bench_duplicates(int n, int loops)
{
	// Empty arrays are not allowed!
	if (n <= 0) return 0;

	struct timespec t_start, t_stop;

	int *array_a = (int *)malloc(n * sizeof(int));
	for (int i = 0; i < n; i++) array_a[i] = rand() % (n * 2);

	int *array_b = (int *)malloc(n * sizeof(int));
	for (int i = 0; i < n; i++) array_b[i] = rand() % (n * 2);

	int sum = 0;

	clock_gettime(CLOCK_MONOTONIC, &t_start);
	for (int i = 0; i < loops; i++)
	{
		for (int a = 0; a < n; a++)
		{
			int a_element = array_a[a];
			for (int b = 0; b < n; b++)
			{
				int b_element = array_b[b];
				if (a_element == b_element)
				{
					sum++;
					break;
				}
			}
		}
	}
	clock_gettime(CLOCK_MONOTONIC, &t_stop);

	// Avoiding memory leaks.
	free(array_a);
	free(array_b);

	// Avoiding compiler optimizations that would give misleading measurements.
	if (sum == 0) return 0;

	return nano_seconds(&t_start, &t_stop);
}

void benchmark(char *filepath, long(*bench)(int, int), int loops_to_find_min, int measurement_loops, int measurements_per_operation, int size_multiplier)
{
	FILE *file = fopen(filepath, "w");
	printf("Opened \"%s\".\n", filepath);

	int *sizes;

	if (size_multiplier != 1)
	{
		measurements_per_operation++;
		sizes = (int *)malloc(measurements_per_operation * sizeof(int));

		sizes[0] = 1;
		for (int i = 1; i < measurements_per_operation; i++)
		{
			sizes[i] = i * size_multiplier;
		}
	}
	else
	{
		sizes = (int *)malloc(measurements_per_operation * sizeof(int));

		for (int i = 0; i < measurements_per_operation; i++)
		{
			sizes[i] = (i + 1) * size_multiplier;
		}
	}

	fprintf(file, "#     size      min      max      avg\n");

	for (int i = 0; i < measurements_per_operation; i++)
	{
		int n = sizes[i];
		double min = DBL_MAX;
		double max = 0;
		double sum = 0;

		for (int minLoop = 0; minLoop < loops_to_find_min; minLoop++)
		{
			long wall = bench(n, measurement_loops);
			double time = (double)wall / (double)measurement_loops;
			if (time < min) min = time;
			if (time > max) max = time;
			sum += time;
		}

		double avg = sum / (double)loops_to_find_min;

		#define TO_PRINT "%10i %8.2f %8.2f %8.2f\n", n, min, max, avg
		printf(TO_PRINT);
		fprintf(file, TO_PRINT);
		#undef TO_PRINT
	}

	free(sizes);

	fclose(file);
	printf("Closed \"%s\".\n", filepath);
}

int main(int argc, char *argv[])
{
	printf("%s %s %s\n", MAKE_CODENAME, MAKE_VERSION, MAKE_DATE);

	srand(time(NULL));

	#define ARGUMENTS_NEEDED 7
	if (argc != 1 + ARGUMENTS_NEEDED)
	{
		printf("Invalid number of arguments!\n");
		printf("How to run:\n");
		printf("\t./path/to/program <1> <2> <3> <4> <5> <6> <7>\n");
		printf("\t1 - loops_to_find_min\n");
		printf("\t2 - measurement_loops\n");
		printf("\t3 - measurements_per_operation\n");
		printf("\t4 - sequential_access_size_multiplier\n");
		printf("\t5 - random_access_size_multiplier\n");
		printf("\t6 - search_size_multiplier\n");
		printf("\t7 - duplicates_size_multiplier\n");
		return EXIT_FAILURE;
	}
	#undef ARGUMENTS_NEEDED

	int loops_to_find_min                 = atoi(argv[1]);
	int measurement_loops                 = atoi(argv[2]);
	int measurements_per_operation        = atoi(argv[3]);
	int sequential_access_size_multiplier = atoi(argv[4]);
	int random_access_size_multiplier     = atoi(argv[5]);
	int search_size_multiplier            = atoi(argv[6]);
	int duplicates_size_multiplier        = atoi(argv[7]);

	printf("Measurement loops: %i\n", measurement_loops);

	struct timespec t_start, t_stop;
	clock_gettime(CLOCK_MONOTONIC, &t_start);

	benchmark("plots/data/plot1.dat", bench_sequential_access, loops_to_find_min, measurement_loops, measurements_per_operation, sequential_access_size_multiplier);
	benchmark("plots/data/plot2.dat", bench_random_access    , loops_to_find_min, measurement_loops, measurements_per_operation, random_access_size_multiplier    );
	benchmark("plots/data/plot3.dat", bench_search           , loops_to_find_min, measurement_loops, measurements_per_operation, search_size_multiplier           );
	benchmark("plots/data/plot4.dat", bench_duplicates       , loops_to_find_min, measurement_loops, measurements_per_operation, duplicates_size_multiplier       );

	clock_gettime(CLOCK_MONOTONIC, &t_stop);
	long benchmark_nano_seconds = nano_seconds(&t_start, &t_stop);
	double benchmark_seconds = (double)benchmark_nano_seconds / NANOSECONDS_PER_SECOND;
	int hrs = benchmark_seconds / 3600;
	int min = (int)fmod(benchmark_seconds, 3600) / 60;
	int sec = (int)fmod(benchmark_seconds, 60);
	printf("Time taken: %02i:%02i:%02i\n", hrs, min, sec);

	return EXIT_SUCCESS;
}
