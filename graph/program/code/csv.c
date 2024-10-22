#include "csv.h"

#include "ANSI.h"
#include "util.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool CSV_DEBUG_MODE = false;

void csv_info_free(csv_info *info, void (*free_element)(void *))
{
	if (free_element != NULL)
	{
		for (size_t i = 0; i < info->array_size; i++)
		{
			free_element(info->array + i * info->sizeof_array_type);
		}
	}
	free(info->array);
}

bool csv_info_read_from_file(csv_info *info, char *filepath, void (*process_value)(void *, char *, unsigned))
{
	if (process_value == NULL)
	{
		printf("No " ANSI_BOLD "process_value" ANSI_OFF " function given...\n");
		return false;
	}

	FILE *file = fopen(filepath, "r");

	if (file == NULL)
	{
		printf(ANSI_BOLD ANSI_RED "Failed to open file: \"%s\"\n" ANSI_OFF, filepath);
		return false;
	}

	printf(ANSI_BOLD ANSI_GREEN "Opened file: \"%s\"\n" ANSI_OFF, filepath);

	size_t line_buffer_size = 0;
	char  *line_buffer = NULL;
	ssize_t current_line_length = 0;

	size_t allocated_array_size = 8;
	info->array = malloc(info->sizeof_array_type * allocated_array_size);

	while ((current_line_length = getline(&line_buffer, &line_buffer_size, file)) != -1)
	{
		if (info->array_size == allocated_array_size)
		{
			if (CSV_DEBUG_MODE)
				printf(ANSI_YELLOW "Trying to expand array...\n" ANSI_OFF);

			expand_array(&info->array, &allocated_array_size, allocated_array_size, info->sizeof_array_type);

			if (CSV_DEBUG_MODE)
				printf(ANSI_YELLOW "Array size expanded to: %lu\n" ANSI_OFF, allocated_array_size);
		}

		void *element = info->array + info->array_size * info->sizeof_array_type;
		char *value = strtok(line_buffer, ",");
		unsigned value_index = 0;
		while (value != NULL)
		{
			process_value(element, value, value_index++);
			value = strtok(NULL, ",");
		}

		info->array_size++;
	}

	free(line_buffer);

	fclose(file);

	printf(ANSI_BOLD ANSI_GREEN "Closed file: \"%s\"\n" ANSI_OFF, filepath);

	return true;
}
