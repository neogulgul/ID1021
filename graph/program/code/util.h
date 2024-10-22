#pragma once

#include <stddef.h>

void expand_array(void **array, size_t *allocated_size, size_t size_expansion, size_t sizeof_array_type);

int random_in_range(int min, int max);
