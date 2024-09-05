#include "stack.h"

bool STACK_STATIC_DEBUG_MODE = false;

stack_static *stack_static_construct(unsigned size)
{
	stack_static *stack = (stack_static *)malloc(sizeof(stack_static));
	stack->array = (int *)malloc(size * sizeof(int));
	stack->top   = 0;
	stack->size  = size;

	if (STACK_STATIC_DEBUG_MODE)
	{
		printf("[%p][static] Stack constructed. (size = %u)\n", stack, size);
	}

	return stack;
}

void stack_static_destruct(stack_static *stack)
{
	free(stack->array);
	free(stack);

	if (STACK_STATIC_DEBUG_MODE)
	{
		printf("[%p][static] Stack destructed.\n", stack);
	}
}

void stack_static_clear(stack_static *stack)
{
	while (stack->top > 0)
	{
		stack_static_pop(stack);
	}
}

void stack_static_push(stack_static *stack, int value)
{
	if (STACK_STATIC_DEBUG_MODE)
	{
		printf("[%p][static] Stack push operation.\n", stack);
		printf("\tStarting configuration: top = %u, size = %u\n", stack->top, stack->size);
	}

	bool out_of_bounds = stack->top == stack->size;
	if (out_of_bounds)
	{
		unsigned lastIndex = stack->size - 1;
		for (int i = 0; i < lastIndex; i++)
		{
			stack->array[i] = stack->array[i + 1];
		}
		stack->top = lastIndex;

		if (STACK_STATIC_DEBUG_MODE)
		{
			printf("\tOut of bounds push.\n");
		}
	}

	stack->array[stack->top++] = value;

	if (STACK_STATIC_DEBUG_MODE)
	{
		printf("\tPushed to stack: %i\n", value);
		printf("\tStopping configuration: top = %u, size = %u\n", stack->top, stack->size);
	}
}

pop_result stack_static_pop(stack_static *stack)
{
	if (STACK_STATIC_DEBUG_MODE)
	{
		printf("[%p][static] Stack pop operation.\n", stack);
		printf("\tStarting configuration: top = %u, size = %u\n", stack->top, stack->size);
	}

	int value;
	bool valid;
	bool out_of_bounds = stack->top == 0;
	if (out_of_bounds)
	{
		value = STACK_OUT_OF_BOUNDS_POP;
		valid = false;

		if (STACK_STATIC_DEBUG_MODE)
		{
			printf("\tOut of bounds pop.\n");
		}
	}
	else
	{
		value = stack->array[--stack->top];
		valid = true;
	}

	if (STACK_STATIC_DEBUG_MODE)
	{
		printf("\tPopped from stack: %i (valid = %s)\n", value, valid ? "true" : "false");
		printf("\tStopping configuration: top = %u, size = %u\n", stack->top, stack->size);
	}

	return (pop_result){
		value,
		valid
	};
}
