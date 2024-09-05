#include "stack.h"

bool STACK_DYNAMIC_DEBUG_MODE = false;

stack_dynamic *stack_dynamic_construct()
{
	unsigned size = DYNAMIC_DEFAULT_SIZE;

	stack_dynamic *stack = (stack_dynamic *)malloc(sizeof(stack_dynamic));
	stack->array = (int *)malloc(size * sizeof(int));
	stack->top   = 0;
	stack->size  = size;

	if (STACK_DYNAMIC_DEBUG_MODE)
	{
		printf("[%p][dynamic] Stack constructed. (size = %u)\n", stack, size);
	}

	return stack;
}

void stack_dynamic_destruct(stack_dynamic *stack)
{
	free(stack->array);
	free(stack);

	if (STACK_DYNAMIC_DEBUG_MODE)
	{
		printf("[%p][dynamic] Stack destructed.\n", stack);
	}
}

void stack_dynamic_clear(stack_dynamic *stack)
{
	while (stack->top > 0)
	{
		stack_dynamic_pop(stack);
	}
}

void stack_dynamic_push(stack_dynamic *stack, int value)
{
	if (STACK_DYNAMIC_DEBUG_MODE)
	{
		printf("[%p][dynamic] Stack push operation.\n", stack);
		printf("\tStarting configuration: top = %u, size = %u\n", stack->top, stack->size);
	}

	bool out_of_bounds = stack->top == stack->size;
	if (out_of_bounds)
	{
		unsigned newSize = stack->size * 2;
		int *expanded = (int *)malloc(newSize * sizeof(int));
		for (unsigned i = 0; i < stack->size; i++)
		{
			expanded[i] = stack->array[i];
		}
		free(stack->array);
		stack->array = expanded;
		stack->size  = newSize;

		if (STACK_DYNAMIC_DEBUG_MODE)
		{
			printf("\tOut of bounds push.\n");
			printf("\tStack size increased to %u.\n", newSize);
		}
	}

	stack->array[stack->top++] = value;

	if (STACK_DYNAMIC_DEBUG_MODE)
	{
		printf("\tPushed to stack: %i\n", value);
		printf("\tStopping configuration: top = %u, size = %u\n", stack->top, stack->size);
	}
}

pop_result stack_dynamic_pop(stack_dynamic *stack)
{
	if (STACK_DYNAMIC_DEBUG_MODE)
	{
		printf("[%p][dynamic] Stack pop operation.\n", stack);
		printf("\tStarting configuration: top = %u, size = %u\n", stack->top, stack->size);
	}

	int value;
	bool valid;
	bool out_of_bounds = stack->top == 0;
	if (out_of_bounds)
	{
		value = STACK_OUT_OF_BOUNDS_POP;
		valid = false;

		if (STACK_DYNAMIC_DEBUG_MODE)
		{
			printf("\tOut of bounds pop.\n");
		}
	}
	else
	{
		value = stack->array[--stack->top];
		valid = true;

		bool shrink_stack = stack->top <= stack->size / 4 && stack->size >= DYNAMIC_DEFAULT_SIZE;
		if (shrink_stack)
		{
			unsigned newSize = stack->size / 2;
			int *shrinked = (int *)malloc(newSize * sizeof(int));
			for (unsigned i = 0; i < newSize; i++)
			{
				shrinked[i] = stack->array[i];
			}
			free(stack->array);
			stack->array = shrinked;
			stack->size  = newSize;

			if (STACK_DYNAMIC_DEBUG_MODE)
			{
				printf("\tStack size decreased to %u.\n", newSize);
			}
		}
	}

	if (STACK_DYNAMIC_DEBUG_MODE)
	{
		printf("\tPopped from stack: %i (valid = %s)\n", value, valid ? "true" : "false");
		printf("\tStopping configuration: top = %u, size = %u\n", stack->top, stack->size);
	}

	return (pop_result){
		value,
		valid
	};
}
