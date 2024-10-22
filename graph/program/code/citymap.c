#include "citymap.h"

#include "ANSI.h"
#include "connection.h"
#include "csv.h"
#include "hashmap.h"

#include <stdio.h>
#include <stdlib.h>

bool CITYMAP_DEBUG_MODE = false;

char *CITYMAP_CITIES[100];
size_t CITYMAP_CITY_COUNT = 0;

static csv_info info = { NULL, 0, sizeof(connection_t) };
static hashmap_t *hashmap = NULL;

void connect_cities(city_t *src, city_t *dst, int minutes)
{
	if (src->destination_count == MAX_DESTINATIONS)
	{
		if (CITYMAP_DEBUG_MODE)
			printf(ANSI_RED "[%p] Max destinations reached." ANSI_OFF, src);
		return;
	}

	src->destinations[src->destination_count++] = (destination_t){ dst->name, minutes };

	if (CITYMAP_DEBUG_MODE)
		printf(ANSI_CYAN "\"%s\" -> \"%s\" : %i minutes\n" ANSI_OFF, src->name, dst->name, minutes);
}

city_t *citymap_set_and_get(char *name)
{
	if (hashmap_get(hashmap, name) == NULL)
	{
		city_t *city = (city_t *)malloc(sizeof(city_t));
		city->name = name;
		city->destination_count = 0;
		hashmap_set(hashmap, name, city);

		CITYMAP_CITIES[CITYMAP_CITY_COUNT++] = name;
	}
	return citymap_get(name);
}

void citymap_load(char *filepath)
{
	if (csv_info_read_from_file(&info, "trains.csv", process_connection))
	{
		const size_t modulo = 100;

		hashmap = hashmap_construct(modulo);

		for (size_t i = 0; i < info.array_size; i++)
		{
			connection_t connection = ((connection_t *)info.array)[i];

			city_t *from = citymap_set_and_get(connection.from);
			city_t *to   = citymap_set_and_get(connection.to);

			connect_cities(from, to, connection.minutes);
			connect_cities(to, from, connection.minutes);
		}

		if (CITYMAP_DEBUG_MODE)
			printf(ANSI_MAGENTA "With modulo = %lu the hashmap encountered %lu collisions.\n" ANSI_OFF, modulo, HASHMAP_COLLISIONS);
	}
}

void citymap_free()
{
	// freeing cities
	for (size_t m = 0; m < hashmap->mod; m++)
	{
		bucket_t *bucket = &hashmap->buckets[m];
		for (size_t p = 0; p < bucket->pair_count; p++)
		{
			pair_t *pair = &bucket->pairs[p];
			free(pair->value);
		}
	}

	hashmap_destruct(hashmap);

	csv_info_free(&info, free_connection);
}

city_t *citymap_get(char *name)
{
	return (city_t *)hashmap_get(hashmap, name);
}
