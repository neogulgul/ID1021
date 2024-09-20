#pragma once

#include <time.h>

#define NANOSECONDS_PER_SECOND 1000000000

typedef struct timespec timespec;

long nano_seconds(timespec *t_start, timespec *t_stop);
