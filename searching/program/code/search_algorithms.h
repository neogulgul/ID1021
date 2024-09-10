#pragma once

#include <stdbool.h>
#include <stddef.h>

bool   unsorted_brute_search(int *array, size_t array_size, int key);
// the following methods assume the array is sorted
bool     sorted_brute_search(int *array, size_t array_size, int key);
bool           binary_search(int *array, size_t array_size, int key);
bool recursive_binary_search(int *array, size_t array_size, int key);
