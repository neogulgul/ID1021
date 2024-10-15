#include "testing.h"

#include "Area.h"
#include "time_measuring.h"

#include <float.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef ZIP_CODE_INTEGER
bool zip_code_linear_search_lookup(Area *areas, size_t area_count, int zip_code)
#else
bool zip_code_linear_search_lookup(Area *areas, size_t area_count, char *zip_code)
#endif
{
	for (size_t i = 0; i < area_count; i++)
	{
		Area area = areas[i];
#ifdef ZIP_CODE_INTEGER
		if (area.zip_code == zip_code) return true;
#else
		if (strcmp(area.zip_code, zip_code) == 0) return true;
#endif
	}
	return false;
}

int compare(const void *a, const void *b)
{
#ifdef ZIP_CODE_INTEGER
	return ((Area *)a)->zip_code - ((Area *)b)->zip_code;
#else
	return zip_code_str_to_int(((Area *)a)->zip_code) - zip_code_str_to_int(((Area *)b)->zip_code);
#endif
}
#ifdef ZIP_CODE_INTEGER
bool zip_code_binary_search_lookup(Area *areas, size_t area_count, int zip_code)
#else
bool zip_code_binary_search_lookup(Area *areas, size_t area_count, char *zip_code)
#endif
{
	return bsearch(&zip_code, areas, area_count, sizeof(Area), compare) != NULL;
}

#ifdef ZIP_CODE_INTEGER
float measure_search_lookup(bool (*lookup)(Area *, size_t, int), Area *areas, size_t area_count, int zip_code)
#else
float measure_search_lookup(bool (*lookup)(Area *, size_t, char *), Area *areas, size_t area_count, char *zip_code)
#endif
{
	timespec t_start, t_stop;
	float min = FLT_MAX;
	for (size_t i = 0; i < 1000; i++)
	{
		const size_t some_iterations = 100;
		clock_gettime(CLOCK_MONOTONIC, &t_start);
		for (size_t j = 0; j < some_iterations; j++)
		{
			lookup(areas, area_count, zip_code);
		}
		clock_gettime(CLOCK_MONOTONIC, &t_stop);
		// printf("Found: %s\n", found ? "true" : "false");
		float time = (float)nano_seconds(&t_start, &t_stop) / (float)some_iterations;
		if (time < min)
			min = time;
	}
	return min;
}

#define AREA_MAP_SIZE 98500
Area area_map[AREA_MAP_SIZE];

#ifdef ZIP_CODE_INTEGER
bool area_map_lookup(int zip_code)
{
	int index = zip_code;
	if (index >= 0 && index < AREA_MAP_SIZE)
	{
		return area_map[index].zip_code == zip_code;
	}
}
#else
bool area_map_lookup(char *zip_code)
{
	int index = zip_code_str_to_int(zip_code);
	if (index >= 0 && index < AREA_MAP_SIZE)
	{
		return strcmp(area_map[index].zip_code, zip_code) == 0;
	}
}
#endif

#ifdef ZIP_CODE_INTEGER
float measure_map_lookup(int zip_code)
#else
float measure_map_lookup(char *zip_code)
#endif
{
	timespec t_start, t_stop;
	float min = FLT_MAX;
	for (size_t i = 0; i < 1000; i++)
	{
		size_t some_iterations = 1000;
		clock_gettime(CLOCK_MONOTONIC, &t_start);
		for (size_t j = 0; j < some_iterations; j++)
		{
			area_map_lookup(zip_code);
		}
		clock_gettime(CLOCK_MONOTONIC, &t_stop);
		// printf("Found: %s\n", found ? "true" : "false");
		float time = (float)nano_seconds(&t_start, &t_stop) / (float)some_iterations;
		if (time < min)
			min = time;
	}
	return min;
}

void testing_a()
{
	csv_info info = { NULL, 0, sizeof(Area) };
	if (csv_info_read_from_file(&info, "postnummer.csv", Area_process_line))
	{
		// printf("Area count: %lu\n", info.array_size);

		for (size_t i = 0; i < info.array_size; i++)
		{
			Area area = ((Area *)info.array)[i];
#ifdef ZIP_CODE_INTEGER
			int index = area.zip_code;
#else
			int index = zip_code_str_to_int(area.zip_code);
#endif
			area_map[index] = area;
		}

		bool found;
#ifdef ZIP_CODE_INTEGER
		found = zip_code_binary_search_lookup(info.array, info.array_size, 11114);
		printf("Found: %s\n", found ? "true" : "false");
		found = zip_code_binary_search_lookup(info.array, info.array_size, 11115);
		printf("Found: %s\n", found ? "true" : "false");
		found = zip_code_binary_search_lookup(info.array, info.array_size, 98499);
		printf("Found: %s\n", found ? "true" : "false");
		found = zip_code_binary_search_lookup(info.array, info.array_size, 98500);
		printf("Found: %s\n", found ? "true" : "false");
#else
		found = zip_code_binary_search_lookup(info.array, info.array_size, "111 14");
		printf("Found: %s\n", found ? "true" : "false");
		found = zip_code_binary_search_lookup(info.array, info.array_size, "111 15");
		printf("Found: %s\n", found ? "true" : "false");
		found = zip_code_binary_search_lookup(info.array, info.array_size, "984 99");
		printf("Found: %s\n", found ? "true" : "false");
		found = zip_code_binary_search_lookup(info.array, info.array_size, "985 00");
		printf("Found: %s\n", found ? "true" : "false");
#endif



// #ifdef ZIP_CODE_INTEGER
// 		printf("Linear:\n");
// 		printf("\t111 15: %.1f\n", measure_search_lookup(zip_code_linear_search_lookup, info.array, info.array_size, 11115));
// 		printf("\t984 99: %.1f\n", measure_search_lookup(zip_code_linear_search_lookup, info.array, info.array_size, 98499));
// 		printf("Binary:\n");
// 		printf("\t111 15: %.1f\n", measure_search_lookup(zip_code_binary_search_lookup, info.array, info.array_size, 11115));
// 		printf("\t984 99: %.1f\n", measure_search_lookup(zip_code_binary_search_lookup, info.array, info.array_size, 98499));
// #else
// 		printf("Linear:\n");
// 		printf("\t111 15: %.1f\n", measure_search_lookup(zip_code_linear_search_lookup, info.array, info.array_size, "111 15"));
// 		printf("\t984 99: %.1f\n", measure_search_lookup(zip_code_linear_search_lookup, info.array, info.array_size, "984 99"));
// 		printf("Binary:\n");
// 		printf("\t111 15: %.1f\n", measure_search_lookup(zip_code_binary_search_lookup, info.array, info.array_size, "111 15"));
// 		printf("\t984 99: %.1f\n", measure_search_lookup(zip_code_binary_search_lookup, info.array, info.array_size, "984 99"));
// #endif



// 	printf("Map:\n");
// #ifdef ZIP_CODE_INTEGER
// 		printf("\t111 15: %.1f\n", measure_map_lookup(11115));
// 		printf("\t984 99: %.1f\n", measure_map_lookup(98499));
// #else
// 		printf("\t111 15: %.1f\n", measure_map_lookup("111 15"));
// 		printf("\t984 99: %.1f\n", measure_map_lookup("984 99"));
// #endif



		csv_info_free(&info, Area_pointer_free);
	}
}
