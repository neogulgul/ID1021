#pragma once

typedef struct
{
	char *from;
	char *to;
	int minutes;
} connection_t;

void process_connection(void *element, char *value, unsigned value_index);

void free_connection(void *element);
