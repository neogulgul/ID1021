#include "connection.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void process_connection(void *element, char *value, unsigned value_index)
{
	connection_t *connection = (connection_t *)element;

	switch (value_index)
	{
		case 0:
		{
			connection->from = strdup(value);
			break;
		}
		case 1:
		{
			connection->to = strdup(value);
			break;
		}
		case 2:
		{
			connection->minutes = atoi(value);
			break;
		}
		default:
		{
			printf("Invalid index!\n");
			break;
		}
	}
}

void free_connection(void *element)
{
	connection_t *connection = (connection_t *)element;
	free(connection->from);
	free(connection->to);
}
