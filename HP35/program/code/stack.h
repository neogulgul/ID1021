#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define STACK_OUT_OF_BOUNDS_POP 0

#define DYNAMIC_DEFAULT_SIZE 8

extern bool STACK_STATIC_DEBUG_MODE;
extern bool STACK_DYNAMIC_DEBUG_MODE;

typedef struct
{
	int value;
	bool valid;
} pop_result;

typedef struct
{
	int *array;
	unsigned top;
	unsigned size;
} stack_static;

stack_static *stack_static_construct(unsigned size);
void          stack_static_destruct (stack_static *stack);
void          stack_static_clear    (stack_static *stack);
void          stack_static_push     (stack_static *stack, int value);
pop_result    stack_static_pop      (stack_static *stack);

typedef struct
{
	int *array;
	unsigned top;
	unsigned size;
} stack_dynamic;

stack_dynamic *stack_dynamic_construct();
void           stack_dynamic_destruct (stack_dynamic *stack);
void           stack_dynamic_clear    (stack_dynamic *stack);
void           stack_dynamic_push     (stack_dynamic *stack, int value);
pop_result     stack_dynamic_pop      (stack_dynamic *stack);
