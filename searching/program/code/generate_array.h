#pragma once

#include <stdlib.h>

typedef enum
{
	ARRAY_TYPE_UNSORTED,
	ARRAY_TYPE_SORTED_SIMPLE,
	ARRAY_TYPE_SORTED,
} ARRAY_TYPE;


int *generate_array(size_t array_size, int min, int max, ARRAY_TYPE array_type);
