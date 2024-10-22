#pragma once

#define MAX_DESTINATIONS 16

#include <stdbool.h>
#include <stddef.h>

extern bool CITYMAP_DEBUG_MODE;

extern char *CITYMAP_CITIES[];
extern size_t CITYMAP_CITY_COUNT;

typedef struct
{
	char *name;
	int minutes;
} destination_t;

typedef struct
{
	char *name;
	destination_t destinations[MAX_DESTINATIONS];
	size_t destination_count;
} city_t;

void    citymap_load(char *filepath);
void    citymap_free();
city_t *citymap_get (char *name);
