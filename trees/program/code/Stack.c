#include "Stack.h"

#include "ANSI.h"

#include <stdio.h>
#include <stdlib.h>

bool STACK_DEBUG_MODE = false;

Cell *Cell_construct(stack_t value)
{
	Cell *cell = (Cell *)malloc(sizeof(Cell));
	cell->value = value;
	cell->next  = NULL;

	if (STACK_DEBUG_MODE)
	{
		printf(ANSI_BOLD ANSI_GREEN "[%p] Cell constructed.\n" ANSI_OFF, cell);
	}

	return cell;
}

void Cell_destruct(Cell *cell)
{
	if (cell == NULL) return;
	Cell_destruct(cell->next);
	free(cell);

	if (STACK_DEBUG_MODE)
	{
		printf(ANSI_BOLD ANSI_RED "[%p] Cell destructed.\n" ANSI_OFF, cell);
	}
}

Stack *Stack_construct()
{
	Stack *stack = (Stack *)malloc(sizeof(Stack));
	stack->top = NULL;

	if (STACK_DEBUG_MODE)
	{
		printf(ANSI_BOLD ANSI_GREEN "[%p] Stack constructed.\n" ANSI_OFF, stack);
	}

	return stack;
}

void Stack_destruct(Stack *stack)
{
	if (stack == NULL) return;
	Cell_destruct(stack->top);
	free(stack);

	if (STACK_DEBUG_MODE)
	{
		printf(ANSI_BOLD ANSI_RED "[%p] Stack destructed.\n" ANSI_OFF, stack);
	}
}

void Stack_clear(Stack *stack)
{
	if (stack == NULL) return;
	Cell_destruct(stack->top);
}

void Stack_push(Stack *stack, stack_t value)
{
	if (stack == NULL) return;
	Cell *cell = Cell_construct(value);
	cell->next = stack->top;
	stack->top = cell;
}

pop_result Stack_pop(Stack *stack)
{
	pop_result result = { NULL, false };

	if (stack != NULL && stack->top != NULL)
	{
		Cell *top = stack->top;
		stack->top = top->next;

		result.value = top->value;
		result.valid = true;

		free(top);
		if (STACK_DEBUG_MODE)
		{
			printf(ANSI_BOLD ANSI_RED "[%p] Cell destructed.\n" ANSI_OFF, top);
		}
	}

	return result;
}
