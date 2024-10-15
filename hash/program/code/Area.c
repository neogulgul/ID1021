#include "Area.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int zip_code_str_to_int(char *zip_code_str)
{
	return atoi(zip_code_str) * 100 + atoi(zip_code_str + 3);
}

void Area_process_line(char *line, csv_info *info)
{
	Area *area = &((Area *)info->array)[info->array_size];

#ifdef ZIP_CODE_INTEGER
	area->zip_code   = zip_code_str_to_int(strdup(strtok(line, ",")));
#else
	area->zip_code   = strdup(strtok(line, ","));
#endif
	area->name       = strdup(strtok(NULL, ","));
	area->population =   atol(strtok(NULL, ","));

	// ugly way to trim trailing whitespaces in the name
	char *c = area->name;
	while (*c != '\0')
	{
		c++;
		if (*c == ' ')
		{
			*c = '\0';
		}
	}
}

void Area_pointer_free(void *p)
{
	Area *area = (Area *)p;
#ifndef ZIP_CODE_INTEGER
	free(area->zip_code);
#endif
	free(area->name);
}
