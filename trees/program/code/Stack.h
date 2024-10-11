#pragma once

#include <stdbool.h>

extern bool STACK_DEBUG_MODE;

typedef void * stack_t;

typedef struct Cell Cell;
struct Cell
{
	stack_t value;
	Cell *next;
};

typedef struct
{
	Cell *top;
} Stack;

typedef struct
{
	stack_t value;
	bool valid;
} pop_result;

Cell *Cell_construct(stack_t value);
void  Cell_destruct (Cell *cell);

Stack     *Stack_construct();
void       Stack_destruct (Stack *stack);
void       Stack_clear    (Stack *stack);
void       Stack_push     (Stack *stack, stack_t value);
pop_result Stack_pop      (Stack *stack);
