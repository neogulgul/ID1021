#include "generate_array.h"

int random_in_range(int min, int max)
{
	int range = max - min + 1;
	return rand() % range + min;
}

int *generate_array_unsorted(size_t array_size, int min, int max)
{
	int *array = (int *)malloc(sizeof(int) * array_size);
	for (size_t i = 0; i < array_size; i++)
	{
		array[i] = random_in_range(min, max);
	}
	return array;
}

int *generate_array_sorted_simple(size_t array_size, int min, int max)
{
	int *array = (int *)malloc(sizeof(int) * array_size);
	int next = min;

	for (size_t i = 0; i < array_size; i++)
	{
		next += random_in_range(0, RAND_MAX / max);
		array[i] = next;
	}

	return array;

	// int *array = (int *)malloc(sizeof(int) * array_size);
	// int next = 0;
	// for (int i = 0; i < array_size; i++)
	// {
	// 	next += rand() % 10 + 1;
	// 	array[i] = next;
	// }
	// return array;
}

int compare_int(const void *a, const void *b)
{
	return *(int *)a - *(int *)b;
}

int *generate_array_sorted(size_t array_size, int min, int max)
{
	int *array = generate_array_unsorted(array_size, min, max);

	qsort(array, array_size, sizeof(int), compare_int);

	return array;
}

int *generate_array(size_t array_size, int min, int max, ARRAY_TYPE array_type)
{
	switch (array_type)
	{
		case ARRAY_TYPE_UNSORTED:      return generate_array_unsorted     (array_size, min, max);
		case ARRAY_TYPE_SORTED_SIMPLE: return generate_array_sorted_simple(array_size, min, max);
		case ARRAY_TYPE_SORTED:        return generate_array_sorted       (array_size, min, max);
	}
}
