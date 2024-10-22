#include "ANSI.h"
#include "citymap.h"
#include "time_measuring.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int find_fastest_path_v1(city_t *src, city_t *dst, int limit)
{
	if (src == NULL || dst == NULL) return -1;
	if (src == dst) return 0;

	int result = -1;

	for (size_t i = 0; i < src->destination_count; i++)
	{
		destination_t nxt = src->destinations[i];

		if (nxt.minutes <= limit)
		{
			int candidate = find_fastest_path_v1(citymap_get(nxt.name), dst, limit - nxt.minutes);
			if (candidate >= 0)
			{
				candidate += nxt.minutes;
				if (result < 0 || candidate < result)
				{
					result = candidate;
				}
			}
		}
	}

	return result;
}

typedef struct cell_t cell_t;
struct cell_t
{
	char *name;
	cell_t *next;
};

cell_t *path_queue = NULL;

void path_push(char *name)
{
	cell_t *new = (cell_t *)malloc(sizeof(cell_t));
	new->name = name;
	new->next = path_queue;
	path_queue = new;
}

void path_pop()
{
	cell_t *popped = path_queue;
	path_queue = popped->next;
	free(popped);
}

bool path_contains(char *name)
{
	cell_t *cell = path_queue;
	while (cell != NULL)
	{
		if (strcmp(cell->name, name) == 0)
		{
			return true;
		}
		cell = cell->next;
	}
	return false;
}

void path_print()
{
	if (path_queue == NULL)
	{
		printf("Empty path.\n");
		return;
	}

	cell_t *cell = path_queue;
	while (cell != NULL)
	{
		printf("%s", cell->name);
		if (cell->next == NULL) break;
		printf(" <- ");
		cell = cell->next;
	}
	printf("\n");
}

void cell_free(cell_t *cell)
{
	if (cell == NULL) return;
	cell_free(cell->next);
	free(cell);
}

int find_fastest_path_v2(city_t *src, city_t *dst, int limit)
{
	if (src == NULL || dst == NULL) return -1;
	if (src == dst) return 0;

	int result = -1;

	path_push(src->name);

	for (size_t i = 0; i < src->destination_count; i++)
	{
		destination_t nxt = src->destinations[i];

		if (!path_contains(nxt.name) && nxt.minutes <= limit)
		{
			int candidate = find_fastest_path_v2(citymap_get(nxt.name), dst, limit - nxt.minutes);
			if (candidate >= 0)
			{
				candidate += nxt.minutes;
				if (result < 0 || candidate < result)
				{
					result = candidate;
				}
			}
		}
	}

	path_pop();

	return result;
}

int find_fastest_path_v3(city_t *src, city_t *dst, int limit)
{
	static const int limit_increment = 50;

	if (src == NULL || dst == NULL) return -1;
	if (src == dst) return 0;

	int result = -1;

	while (result == -1)
	{
		result = find_fastest_path_v2(src, dst, limit += limit_increment);
	}

	return result;
}

int (*trip_shortest_path_algorithm)(city_t *, city_t *, int) = NULL;
bool trip_print_stuff = true;

typedef struct
{
	char *src_name, *dst_name;
	int limit, fastest_time;
	float elapsed_ms;
} trip_info_t;

trip_info_t trip(char *src_name, char *dst_name, int limit)
{
	trip_info_t info = {
		src_name,
		dst_name,
		limit,
		-1,
		0
	};

	if (trip_shortest_path_algorithm == NULL)
	{
		printf("No " ANSI_BOLD "shortest path algorithm" ANSI_OFF " specified!\n");
		goto TRIP_RETURN;
	}

	city_t *src = citymap_get(src_name);
	bool valid_src = src != NULL;
	if (!valid_src) printf("Cound not find \"%s\"...\n", src_name);

	city_t *dst = citymap_get(dst_name);
	bool valid_dst = dst != NULL;
	if (!valid_dst) printf("Cound not find \"%s\"...\n", dst_name);

	if (!valid_src || !valid_dst) goto TRIP_RETURN;;

	if (trip_print_stuff)
	{
		printf("%s -> %s, Time limit: %i minutes\n", src_name, dst_name, limit);
		printf("Searching...\n");
	}

	timespec t_start, t_stop;
	clock_gettime(CLOCK_MONOTONIC, &t_start);
	int fastest_time = trip_shortest_path_algorithm(src, dst, limit);
	clock_gettime(CLOCK_MONOTONIC, &t_stop);
	long elapsed = nano_seconds(&t_start, &t_stop);

	float elapsed_ms = (float)elapsed / 1000000.f;

	info.fastest_time = fastest_time;
	info.elapsed_ms = elapsed_ms;

	if (trip_print_stuff)
	{
		printf("Elapsed time: %.1f ms\n", elapsed_ms);

		if (fastest_time == -1)
		{
			printf(ANSI_BOLD ANSI_RED "No shortest path found (%s -> %s, Time limit: %i minutes).\n" ANSI_OFF, src_name, dst_name, limit);
		}
		else
		{
			printf(ANSI_BOLD ANSI_GREEN "Shortest path found takes %i minutes (%s -> %s).\n" ANSI_OFF, fastest_time, src_name, dst_name);
		}
	}

TRIP_RETURN:
	return info;
}

void print_destinations(char *city_name)
{
	city_t *city = citymap_get(city_name);

	if (city == NULL)
	{
		printf("Could not find \"%s\"...\n");
		return;
	}

	printf("\"%s\" -> ", city_name);
	if (city->destination_count > 0)
	{
		printf("\"%s\"", city->destinations[0].name);
		for (size_t i = 1; i < city->destination_count; i++)
		{
			printf(", \"%s\"", city->destinations[i].name);
		}
	}
	printf("\n");
}

int main(int argc, char *argv[])
{
	printf("%s %s %s\n", MAKE_CODENAME, MAKE_VERSION, MAKE_DATE);

	citymap_load("trains.csv");

	// if (argc != 4)
	// {
	// 	printf(ANSI_BOLD "USAGE\n" ANSI_OFF);
	// 	printf("\t%s <from> <to> <limit>\n", argv[0]);
	// 	printf(ANSI_BOLD "NOTES\n" ANSI_OFF);
	// 	printf("\t<from>, <to> - City names.\n");
	// 	printf("\t<limit>      - Time limit specified in minutes.\n");
	// 	return EXIT_FAILURE;
	// }

	// char *src_name =      argv[1];
	// char *dst_name =      argv[2];
	// int      limit = atoi(argv[3]);

	// trip(src_name, dst_name, limit);



	// trip_shortest_path_algorithm = find_fastest_path_v1;
	// printf(ANSI_BOLD ANSI_YELLOW "V1\n" ANSI_OFF);
	// trip(    "Malmö",  "Göteborg", 300);
	// trip( "Göteborg", "Stockholm", 300);
	// trip(    "Malmö", "Stockholm", 300);
	// trip("Stockholm", "Sundsvall", 350);
	// trip("Stockholm",      "Umeå", 550);
	// trip( "Göteborg", "Sundsvall", 550);
	// trip("Sundsvall",      "Umeå", 300);
	// trip(     "Umeå",  "Göteborg", 800);
	// trip( "Göteborg",      "Umeå", 550);

	// trip_shortest_path_algorithm = find_fastest_path_v2;
	// printf(ANSI_BOLD ANSI_YELLOW "V2\n" ANSI_OFF);
	// trip(    "Malmö",  "Göteborg", 300);
	// trip( "Göteborg", "Stockholm", 300);
	// trip(    "Malmö", "Stockholm", 300);
	// trip("Stockholm", "Sundsvall", 400);
	// trip("Stockholm",      "Umeå", 800);
	// trip( "Göteborg", "Sundsvall", 800);
	// trip("Sundsvall",      "Umeå", 300);
	// trip(     "Umeå",  "Göteborg", 800);
	// trip( "Göteborg",      "Umeå", 800);

	// trip_shortest_path_algorithm = find_fastest_path_v3;
	// printf(ANSI_BOLD ANSI_YELLOW "V3\n" ANSI_OFF);
	// trip(    "Malmö",  "Göteborg", 0);
	// trip( "Göteborg", "Stockholm", 0);
	// trip(    "Malmö", "Stockholm", 0);
	// trip("Stockholm", "Sundsvall", 0);
	// trip("Stockholm",      "Umeå", 0);
	// trip( "Göteborg", "Sundsvall", 0);
	// trip("Sundsvall",      "Umeå", 0);
	// trip(     "Umeå",  "Göteborg", 0);
	// trip( "Göteborg",      "Umeå", 0);



	trip_shortest_path_algorithm = find_fastest_path_v3;
	trip_print_stuff = false;
	char *origin = "Malmö";
	printf(ANSI_CYAN "From \"%s\" to ...\n" ANSI_OFF, origin);
	for (size_t i = 0; i < CITYMAP_CITY_COUNT; i++)
	{
		char *destination = CITYMAP_CITIES[i];
		if (strcmp(origin, destination) == 0)
		{
		}
		else
		{
			trip_info_t info = trip(origin, destination, 0);
			printf("%s & %i & %.2f \\\\\n", info.dst_name, info.fastest_time, info.elapsed_ms);
		}
	}

	citymap_free();

	cell_free(path_queue);

	return EXIT_SUCCESS;
}
