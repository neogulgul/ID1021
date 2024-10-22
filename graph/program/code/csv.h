#pragma once

#include <stdbool.h>
#include <stddef.h>

extern bool CSV_DEBUG_MODE;

typedef struct
{
	void  *array;
	size_t array_size;
	size_t sizeof_array_type;
} csv_info;

void csv_info_free(csv_info *info, void (*free_element)(void *));

bool csv_info_read_from_file(csv_info *info, char *filepath, void (*process_value)(void *, char *, unsigned));
