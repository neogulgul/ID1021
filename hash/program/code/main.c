#include "ANSI.h"
#include "testing.h"

#include <stdio.h>

// I have no idea what I am doing.

int main(int argc, char *argv[])
{
	printf("%s %s %s\n", MAKE_CODENAME, MAKE_VERSION, MAKE_DATE);

#ifdef ZIP_CODE_INTEGER
	printf(ANSI_BOLD ANSI_MAGENTA "Zip codes are represented by integers.\n" ANSI_OFF);
#else
	printf(ANSI_BOLD ANSI_MAGENTA "Zip codes are represented by strings.\n" ANSI_OFF);
#endif

	testing_a();
#ifdef ZIP_CODE_INTEGER
	testing_b();
	testing_c();
	testing_d();
#endif

	return 0;
}
