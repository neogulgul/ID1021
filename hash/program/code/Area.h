#pragma once

#include "csv.h"

typedef struct
{
#ifdef ZIP_CODE_INTEGER
	int zip_code;
#else
	char *zip_code;
#endif
	char *name;
	long population;
} Area;

int zip_code_str_to_int(char *zip_code_str);

void Area_process_line(char *line, csv_info *info);

void Area_pointer_free(void *p);
