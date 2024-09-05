#include "calculator.h"
#include "tests.h"

#include <stdio.h>

int main(int argc, char *argv[])
{
	printf("%s %s %s\n", MAKE_CODENAME, MAKE_VERSION, MAKE_DATE);

	tests();
	calculator();

	return 0;
}
