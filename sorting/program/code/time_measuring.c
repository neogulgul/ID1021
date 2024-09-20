#include "time_measuring.h"

long nano_seconds(timespec *t_start, timespec *t_stop)
{
	return (t_stop->tv_nsec - t_start->tv_nsec) +
	       (t_stop->tv_sec - t_start->tv_sec) * NANOSECONDS_PER_SECOND;
}
