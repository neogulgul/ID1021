#include "Queue.h"
#include "time_measuring.h"

#include <stdio.h>

Queue *root = NULL;

void prep(size_t n)
{
	root = Queue_construct();
	for (int i = 0; i < n; i++)
	{
		Queue_enqueue(root, i);
	}
}

void dequeue(size_t n)
{
	Queue_dequeue(root);
}

void enqueue(size_t n)
{
	Queue_enqueue(root, 0);
}

void enqueue_optimized(size_t n)
{
	Queue_enqueue_optimized(root, 0);
}

void clean_up(size_t n)
{
	Queue_destruct(root);
}

void modify_measurements(measurement_info *measurements)
{
}

size_t size_formula(size_t n)
{
	return n * 10;
}

void testing()
{
	Queue *test = Queue_construct();
	for (int i = 0; i < 10; i++)
	{
		Queue_enqueue_optimized(test, i);
		Queue_print(test);
	}
	for (int i = 0; i < 10; i++)
	{
		Queue_dequeue(test);
		Queue_print(test);
	}
	Queue_destruct(test);
}

int main(int argc, char *argv[])
{
	printf("%s %s %s\n", MAKE_CODENAME, MAKE_VERSION, MAKE_DATE);

	// testing();

	record_benchmark("plots/data/dequeue.dat", prep, dequeue, clean_up, modify_measurements, size_formula, 100);
	record_benchmark("plots/data/enqueue.dat", prep, enqueue, clean_up, modify_measurements, size_formula, 100);
	record_benchmark("plots/data/enqueue_optimized.dat", prep, enqueue_optimized, clean_up, modify_measurements, size_formula, 100);

	return 0;
}
