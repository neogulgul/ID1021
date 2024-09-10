#include "search_algorithms.h"

#include <math.h>
#include <stdio.h>

bool unsorted_brute_search(int *array, size_t array_size, int key)
{
	for (size_t i = 0; i < array_size; i++)
	{
		if (key == array[i])
		{
			return true;
		}
	}
	return false;
}

bool sorted_brute_search(int *array, size_t array_size, int key)
{
	for (size_t i = 0; i < array_size; i++)
	{
		int item = array[i];
		if (key == item)
		{
			return true;
		}
		else if (key < item)
		{
			return false;
		}
	}
	return false;
}

bool binary_search(int *array, size_t array_size, int key)
{
	size_t first = 0;
	size_t last  = array_size - 1;

	while (first <= last)
	{
		size_t middle = first + (last - first) / 2;

		int item = array[middle];

		if (key == item) return true;

		if (key > item)
		{
			if (middle == array_size - 1) break;
			first = middle + 1;
		}
		else
		{
			if (middle == 0) break;
			last = middle - 1;
		}
	}

	return false;
}

// have to use this helper function since
// i want all search functions that i benchmark to take the same arguments
// to make it easier for myself
bool recursive_binary_search_helper(int *array, size_t array_size, int key, size_t first, size_t last)
{
	size_t middle = first + (last - first) / 2;
	int item = array[middle];

	if (key == item) return true;

	if (key > item)
	{
		if (middle < array_size - 1)
		{
			return recursive_binary_search_helper(array, array_size, key, middle + 1, last);
		}
	}
	else
	{
		if (middle > 0)
		{
			return recursive_binary_search_helper(array, array_size, key, first, middle - 1);
		}
	}

	return false;
}

bool recursive_binary_search(int *array, size_t array_size, int key)
{
	return recursive_binary_search_helper(array, array_size, key, 0, array_size - 1);
}
