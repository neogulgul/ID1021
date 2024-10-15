#ifdef ZIP_CODE_INTEGER

#include "testing.h"

#include "ANSI.h"
#include "Area.h"
#include "time_measuring.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MODULO 19412
#define BUCKET_STARTING_SIZE 8

typedef struct
{
	Area *areas;
	size_t area_count;
	size_t allocated_size;
} Bucket;

static int random_in_range(int min, int max)
{
	int range = max - min + 1;
	return rand() % range + min;
}

static Bucket bucket_map[MODULO];

static csv_info info = { NULL, 0, sizeof(Area) };

static int zip_code_hash(int zip_code)
{
	return zip_code % MODULO;
}

bool bucket_lookup(int zip_code)
{
	Bucket bucket = bucket_map[zip_code_hash(zip_code)];
	for (size_t i = 0; i < bucket.area_count; i++)
	{
		if (bucket.areas[i].zip_code == zip_code)
		{
			return true;
		}
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
		bucket_lookup(n);
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

static float average_collisions(size_t modulo)
{
	Bucket *map = (Bucket *)malloc(sizeof(Bucket) * modulo);

	// creating buckets
	for (size_t m = 0; m < modulo; m++)
	{
		Bucket *bucket = &map[m];
		bucket->areas = (Area *)malloc(sizeof(Area) * BUCKET_STARTING_SIZE);
		bucket->area_count = 0;
		bucket->allocated_size = BUCKET_STARTING_SIZE;
	}

	// populating hash map
	for (size_t i = 0; i < info.array_size; i++)
	{
		Area area = ((Area *)info.array)[i];
		int hash = area.zip_code % modulo;
		Bucket *bucket = &map[hash];
		bucket->area_count++; // don't care about actually populating the array
	}

	// calculating the average
	size_t total = 0, non_empty_buckets = 0;
	for (size_t m = 0; m < modulo; m++)
	{
		Bucket *bucket = &map[m];
		if (bucket->area_count > 0)
		{
			total += bucket->area_count;
			non_empty_buckets++;
		}
	}

	// printf("Total             : %lu\n", total);
	// printf("Non empty buckets : %lu\n", non_empty_buckets);

	// freeing stuff
	for (size_t m = 0; m < modulo; m++)
	{
		Bucket *bucket = &map[m];
		free(bucket->areas);
	}
	free(map);

	return (float)total / (float)non_empty_buckets;
}

static void modify_measurements_for_average_collisions(measurement_info *measurements)
{
	measurements->min = average_collisions(measurements->n);
}

static size_t size_formula_for_average_collisions(size_t n)
{
	return n * 10;
}

void testing_c()
{
	for (size_t m = 0; m < MODULO; m++)
	{
		Bucket *bucket = &bucket_map[m];
		bucket->areas = (Area *)malloc(sizeof(Area) * BUCKET_STARTING_SIZE);
		bucket->area_count = 0;
		bucket->allocated_size = BUCKET_STARTING_SIZE;
	}

	if (csv_info_read_from_file(&info, "postnummer.csv", Area_process_line))
	{
		// populating hash map
		for (size_t i = 0; i < info.array_size; i++)
		{
			Area area = ((Area *)info.array)[i];
			int hash = zip_code_hash(area.zip_code);
			Bucket *bucket = &bucket_map[hash];
			if (bucket->area_count == bucket->allocated_size)
			{
				expand_array((void **)&bucket->areas, &bucket->allocated_size, bucket->allocated_size, sizeof(Area));
			}
			bucket->areas[bucket->area_count++] = area;
		}

		// bool found;
		// found = bucket_lookup(11114);
		// printf("Found: %s\n", found ? "true" : "false");
		// found = bucket_lookup(11115);
		// printf("Found: %s\n", found ? "true" : "false");
		// found = bucket_lookup(98499);
		// printf("Found: %s\n", found ? "true" : "false");
		// found = bucket_lookup(98500);
		// printf("Found: %s\n", found ? "true" : "false");

		record_benchmark("plots/data/bucket_lookup.dat", function_prep, function, function_clean_up, modify_measurements, size_formula, 100);

		// printf(ANSI_BOLD ANSI_MAGENTA "HERE\n" ANSI_OFF);
		// printf("Average for 12345: %f\n", average_collisions(12345));

		record_benchmark("plots/data/bucket_average.dat", NULL, NULL, NULL, modify_measurements_for_average_collisions, size_formula_for_average_collisions, 100);

		printf(ANSI_BOLD ANSI_CYAN "Average collisions for %lu: %f\n" ANSI_OFF, MODULO, average_collisions(MODULO));

		csv_info_free(&info, Area_pointer_free);
	}

	for (size_t m = 0; m < MODULO; m++)
	{
		Bucket *bucket = &bucket_map[m];
		free(bucket->areas);
	}
}

#endif // ZIP_CODE_INTEGER
