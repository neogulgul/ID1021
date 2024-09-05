#include "util.h"

void print_array_of_ints(int *array, unsigned size)
{
	printf("{ ");
	if (size > 0)
	{
		printf("%i", array[0]);
		for (int i = 1; i < size; i++)
		{
			printf(", %i", array[i]);
		}
	}
	printf(" }\n");
}

bool str_is_int(char *str)
{
	if (str[0] == '-' || str[0] == '+') str++;
	while (*str != '\0')
	{
		bool is_int = *str >= '0' && *str <= '9';
		if (!is_int)
		{
			return false;
		}
		str++;
	}
	return true;
}
