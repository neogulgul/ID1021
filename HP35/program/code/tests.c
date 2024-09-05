#include "tests.h"

#include "util.h"
#include "stack.h"

void test_static(unsigned operations)
{
	STACK_STATIC_DEBUG_MODE = true;
	stack_static *stack = stack_static_construct(operations);

	for (unsigned i = 0; i < operations + 1; i++)
	{
		stack_static_push(stack, i);
		print_array_of_ints(stack->array, stack->top);
	}
	for (unsigned i = 0; i < operations + 1; i++)
	{
		stack_static_pop(stack);
		print_array_of_ints(stack->array, stack->top);
	}

	stack_static_destruct(stack);
	STACK_STATIC_DEBUG_MODE = false;
}

void test_dynamic(unsigned operations)
{
	STACK_DYNAMIC_DEBUG_MODE = true;
	stack_dynamic *stack = stack_dynamic_construct();

	for (unsigned i = 0; i < operations; i++)
	{
		stack_dynamic_push(stack, i);
	}
	for (unsigned i = 0; i < operations + 1; i++)
	{
		stack_dynamic_pop(stack);
	}

	stack_dynamic_destruct(stack);
	STACK_DYNAMIC_DEBUG_MODE = false;
}

void tests()
{
	printf("TESTS STARTED\n");
	test_static(8);
	test_dynamic(128);
	printf("TESTS COMPLETE\n");
}
