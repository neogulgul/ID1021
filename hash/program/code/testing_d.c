#ifdef ZIP_CODE_INTEGER

#include "testing.h"

#include "ANSI.h"
#include "Area.h"
#include "time_measuring.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static csv_info info = { NULL, 0, sizeof(Area) };

static int random_in_range(int min, int max)
{
	int range = max - min + 1;
	return rand() % range + min;
}

#define MODULO 19412
#define MAP_SIZE MODULO * 2
static Area *hash_map[MAP_SIZE];

static int zip_code_hash(int zip_code)
{
	return zip_code % MODULO;
}

static bool probing_lookup(int zip_code)
{
	int hash = zip_code_hash(zip_code);

	for (size_t i = hash; i < MAP_SIZE; i++)
	{
		Area *spot = hash_map[i];
		if (spot == NULL) return false;
		if (spot->zip_code == zip_code) return true;
	}

	return false;
}

static const size_t iterations = 10000;

static void function_prep(size_t n)
{
}

static void function(size_t n)
{
	for (size_t i = 0; i < iterations; i++)
	{
		probing_lookup(n);
	}
}

static void function_clean_up(size_t n)
{
}

static void modify_measurements(measurement_info *measurements)
{
	measurements->min = measurements->min / iterations;
	measurements->max = measurements->max / iterations;
	measurements->sum = measurements->sum / iterations;
	measurements->avg = measurements->avg / iterations;
}

static size_t size_formula(size_t n)
{
	return n * 1000;
}

static float probing_average(size_t modulo)
{
	size_t map_size = modulo * 2;
	Area **map = (Area **)malloc(sizeof(Area *) * map_size);

	for (size_t i = 0; i < map_size; i++)
	{
		map[i] = NULL;
	}

	// populating hash map
	for (size_t i = 0; i < info.array_size; i++)
	{
		Area *area = &((Area *)info.array)[i];
		int hash = area->zip_code % modulo;
		Area **spot = &map[hash];
		for (size_t i = hash; i < map_size && *spot == NULL; i++)
		{
			(*spot)++;
		}
		if (*spot == NULL)
		{
			printf(ANSI_BOLD ANSI_RED "THE MAP IS FULL!!!\n" ANSI_OFF);
		}
		else
		{
			*spot = area;
		}
	}

	size_t total_go_throughs = 0;
	for (size_t i = 0; i < info.array_size; i++)
	{
		if (i >= map_size)
		{
			total_go_throughs += map_size;
			break;
		}

		Area *area = &((Area *)info.array)[i];
		int hash = area->zip_code % modulo;

		for (size_t h = hash; h < map_size; h++)
		{
			Area **spot = &map[h];
			if (*spot == NULL || *spot == area) break;
			total_go_throughs++;
		}
	}

	free(map);

	return (float)total_go_throughs / (float)info.array_size;
}

static void modify_measurements_for_probing_average(measurement_info *measurements)
{
	measurements->min = probing_average(measurements->n);
}

static size_t size_formula_for_probing_average(size_t n)
{
	return 19000 + n * 10;
}

void testing_d()
{
	for (size_t i = 0; i < MAP_SIZE; i++)
	{
		hash_map[i] = NULL;
	}

	if (csv_info_read_from_file(&info, "postnummer.csv", Area_process_line))
	{
		// populating hash map
		for (size_t i = 0; i < info.array_size; i++)
		{
			Area *area = &((Area *)info.array)[i];
			int hash = zip_code_hash(area->zip_code);

			bool found_empty_spot = false;

			for (size_t h = hash; h < MAP_SIZE; h++)
			{
				Area **spot = &hash_map[h];
				if (*spot == NULL)
				{
					found_empty_spot = true;
					*spot = area;
					break;
				}
			}

			if (!found_empty_spot)
			{
				printf(ANSI_BOLD ANSI_RED "THE MAP IS FULL!!!\n" ANSI_OFF);
			}
		}

		// bool found;
		// found = probing_lookup(11114);
		// printf("Found: %s\n", found ? "true" : "false");
		// found = probing_lookup(11115);
		// printf("Found: %s\n", found ? "true" : "false");
		// found = probing_lookup(98499);
		// printf("Found: %s\n", found ? "true" : "false");
		// found = probing_lookup(98500);
		// printf("Found: %s\n", found ? "true" : "false");

		record_benchmark("plots/data/probing_lookup.dat", function_prep, function, function_clean_up, modify_measurements, size_formula, 100);

		record_benchmark("plots/data/probing_average.dat", NULL, NULL, NULL, modify_measurements_for_probing_average, size_formula_for_probing_average, 100);

		size_t total_loop_checks = 0;
		for (size_t i = 0; i < info.array_size; i++)
		{
			Area *area = &((Area *)info.array)[i];
			int hash = zip_code_hash(area->zip_code);
			for (size_t h = hash; h < MAP_SIZE; h++)
			{
				Area *spot = hash_map[h];
				total_loop_checks++;
				if (spot == NULL || spot == area) break;
			}
		}
		float average_loop_checks = (float)total_loop_checks / (float)info.array_size;
		printf(ANSI_BOLD ANSI_CYAN "Average number of loop checks: %f\n" ANSI_OFF, average_loop_checks);

		csv_info_free(&info, Area_pointer_free);
	}
}

#endif // ZIP_CODE_INTEGER
