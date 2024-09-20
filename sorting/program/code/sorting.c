#include "sorting.h"

#include <stdbool.h>
#include <stdlib.h>

void swap(int *a, int *b)
{
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

void useless_sort(int *array, size_t size)
{
}

void selection_sort(int *array, size_t size)
{
	if (size == 0) return;

	for (size_t i = 0; i < size - 1; i++)
	{
		size_t min_value_index = i;
		for (size_t j = i + 1; j < size; j++)
		{
			if (array[j] < array[min_value_index]) min_value_index = j;
		}
		if (min_value_index != i)
		{
			swap(&array[min_value_index], &array[i]);
		}
	}
}

void no_check_selection_sort(int *array, size_t size)
{
	if (size == 0) return;

	for (size_t i = 0; i < size - 1; i++)
	{
		size_t min_value_index = i;
		for (size_t j = i + 1; j < size; j++)
		{
			if (array[j] < array[min_value_index]) min_value_index = j;
		}
		swap(&array[min_value_index], &array[i]);
	}
}

void insertion_sort(int *array, size_t size)
{
	for (size_t i = 1; i < size; i++)
	{
		for (size_t j = i; j > 0 && array[j] < array[j - 1]; j--)
		{
			swap(&array[j], &array[j - 1]);
		}
	}
}

void merge_sort_merge(int *array, int *tmp, size_t lo, size_t mid, size_t hi)
{
	for (size_t i = lo; i <= hi; i++)
	{
		tmp[i] = array[i];
	}

	size_t left_half_start = lo;
	size_t right_half_start = mid + 1;
	for (size_t i = lo; i <= hi; i++)
	{
		if (left_half_start > mid)
		{
			array[i] = tmp[right_half_start++];
		}
		else if (right_half_start > hi)
		{
			array[i] = tmp[left_half_start++];
		}
		else if (tmp[left_half_start] < tmp[right_half_start])
		{
			array[i] = tmp[left_half_start++];
		}
		else
		{
			array[i] = tmp[right_half_start++];
		}
	}
}

void merge_sort_recursion(int *array, int *tmp, size_t lo, size_t hi)
{
	if (lo >= hi) return;

	size_t mid = (lo + hi) / 2;
	merge_sort_recursion(array, tmp, lo, mid);
	merge_sort_recursion(array, tmp, mid + 1, hi);
	merge_sort_merge(array, tmp, lo, mid, hi);
}

void merge_sort(int *array, size_t size)
{
	if (size <= 1) return;

	int *tmp = (int *)malloc(sizeof(int) * size);

	merge_sort_recursion(array, tmp, 0, size - 1);

	free(tmp);
}
