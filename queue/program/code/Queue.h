#pragma once

#include <stdbool.h>

extern bool QUEUE_DEBUG_PRINT;

typedef struct Cell Cell;
struct Cell
{
	int value;
	Cell *next;
};

Cell *Cell_construct         (int value);
void  Cell_destruct          (Cell *cell);
void  Cell_destruct_recursive(Cell *cell);

typedef struct
{
	Cell *first;
	Cell *last;
} Queue;

typedef struct
{
	int value;
	bool valid;
} dequeue_result;

Queue         *Queue_construct        ();
void           Queue_destruct         (Queue *queue);
void           Queue_print            (Queue *queue);
void           Queue_clear            (Queue *queue);
void           Queue_enqueue          (Queue *queue, int value);
void           Queue_enqueue_optimized(Queue *queue, int value);
dequeue_result Queue_dequeue          (Queue *queue);
