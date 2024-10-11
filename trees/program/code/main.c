#include "ANSI.h"
#include "time_measuring.h"
#include "Tree.h"
#include "Stack.h"

#include <stdio.h>
#include <stdlib.h>

int random_in_range(int min, int max)
{
	int range = max - min + 1;
	return rand() % range + min;
}

Tree *benchmark_tree = NULL;

#define ITERATIONS 1000

void function_prep(size_t n)
{
	benchmark_tree = Tree_construct();
	for (size_t i = 0; i < n; i++)
	{
		Tree_add(benchmark_tree, random_in_range(0, RAND_MAX));
	}
}

void function_add(size_t n)
{
	TREE_DUMMY_ADD = true;
	for (size_t i = 0; i < ITERATIONS; i++)
	{
		Tree_add(benchmark_tree, random_in_range(0, RAND_MAX));
	}
	TREE_DUMMY_ADD = false;
}

void function_add_recursive(size_t n)
{
	TREE_DUMMY_ADD = true;
	for (size_t i = 0; i < ITERATIONS; i++)
	{
		Tree_add_recursive(benchmark_tree, random_in_range(0, RAND_MAX));
	}
	TREE_DUMMY_ADD = false;
}

void function_lookup(size_t n)
{
	for (size_t i = 0; i < ITERATIONS; i++)
	{
		Tree_lookup(benchmark_tree, random_in_range(0, RAND_MAX));
	}
}

void function_clean_up(size_t n)
{
	Tree_destruct(benchmark_tree);
}

void modify_measurements(measurement_info *measurements)
{
	measurements->min /= ITERATIONS;
	measurements->max /= ITERATIONS;
	measurements->sum /= ITERATIONS;
	measurements->avg /= ITERATIONS;
}

size_t size_formula(size_t n)
{
	return n * 100;
}

void ANSI_COLOR_TEST()
{
	printf(
		ANSI_RED     "RED"
		ANSI_GREEN   "GREEN"
		ANSI_YELLOW  "YELLOW"
		ANSI_BLUE    "BLUE"
		ANSI_MAGENTA "MAGENTA"
		ANSI_CYAN    "CYAN"
		ANSI_OFF
		ANSI_BOLD    "BOLD"
		ANSI_OFF
		"\n"
	);
}

void tree_print_test()
{
	printf(ANSI_BOLD ANSI_MAGENTA "[tree_print_test] Start!\n" ANSI_OFF);

	Tree *tree = Tree_construct();

	/*
		      4
		   /     \
		  2       6
		 / \     / \
		1   3   5   7

		1 2 3 4 5 6 7?
	*/

	Tree_add(tree, 4);
	Tree_add(tree, 2);
	Tree_add(tree, 1);
	Tree_add(tree, 3);
	Tree_add(tree, 6);
	Tree_add(tree, 5);
	Tree_add(tree, 7);

	Tree_print(tree);

	Tree_destruct(tree);

	printf(ANSI_BOLD ANSI_CYAN "[tree_print_test] Stop!\n" ANSI_OFF);
}

void stack_test()
{
	printf(ANSI_BOLD ANSI_MAGENTA "[stack_test] Start!\n" ANSI_OFF);

	STACK_DEBUG_MODE = true;

	Stack *stack = Stack_construct();

	float f = 3.14;
	int   i = 1337;
	char *s = "Hello, World!";

	Stack_push(stack, &f);
	Stack_push(stack, &i);
	Stack_push(stack,  s);

	printf("s: %s\n", Stack_pop(stack).value);
	printf("i: %i\n", *(int *)Stack_pop(stack).value);
	printf("f: %f\n", *(float *)Stack_pop(stack).value);

	Stack_destruct(stack);

	STACK_DEBUG_MODE = false;

	printf(ANSI_BOLD ANSI_CYAN "[stack_test] Stop!\n" ANSI_OFF);
}

void tree_print_with_explicit_stack_test()
{
	printf(ANSI_BOLD ANSI_MAGENTA "[tree_print_with_explicit_stack_test] Start!\n" ANSI_OFF);

	STACK_DEBUG_MODE = true;

	Tree *tree = Tree_construct();

	/*
		      4
		   /     \
		  2       6
		 / \     / \
		1   3   5   7

		1 2 3 4 5 6 7?
	*/

	Tree_add(tree, 4);
	Tree_add(tree, 2);
	Tree_add(tree, 1);
	Tree_add(tree, 3);
	Tree_add(tree, 6);
	Tree_add(tree, 5);
	Tree_add(tree, 7);

	Tree_print_with_explicit_stack(tree);

	Tree_destruct(tree);

	STACK_DEBUG_MODE = false;

	printf(ANSI_BOLD ANSI_CYAN "[tree_print_with_explicit_stack_test] Stop!\n" ANSI_OFF);
}

int main(int argc, char *argv[])
{
	printf("%s %s %s\n", MAKE_CODENAME, MAKE_VERSION, MAKE_DATE);

	// ANSI_COLOR_TEST();
	// tree_print_test();
	// stack_test();
	// tree_print_with_explicit_stack_test();

	record_benchmark("plots/data/add.dat", function_prep, function_add, function_clean_up, modify_measurements, size_formula, 100);
	record_benchmark("plots/data/lookup.dat", function_prep, function_lookup, function_clean_up, modify_measurements, size_formula, 100);
	record_benchmark("plots/data/add_recursive.dat", function_prep, function_add_recursive, function_clean_up, modify_measurements, size_formula, 100);

	return 0;
}
