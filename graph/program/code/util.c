#include "util.h"

#include <stdlib.h>
#include <string.h>

void expand_array(void **array, size_t *allocated_size, size_t size_expansion, size_t sizeof_array_type)
{
	size_t new_array_size = *allocated_size + size_expansion;
	void *new_array = malloc(sizeof_array_type * new_array_size);

	memcpy(new_array, *array, sizeof_array_type * *allocated_size);

	*allocated_size = new_array_size;

	free(*array);
	*array = new_array;
}

int random_in_range(int min, int max)
{
	int range = max - min + 1;
	return rand() % range + min;
}
