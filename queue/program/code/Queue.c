#include "Queue.h"

#include "ANSI.h"

#include <stdio.h>
#include <stdlib.h>

bool QUEUE_DEBUG_PRINT = false;

Cell *Cell_construct(int value)
{
	Cell *cell = (Cell *)malloc(sizeof(Cell));

	cell->value = value;
	cell->next  = NULL;

	if (QUEUE_DEBUG_PRINT)
	{
		printf(ANSI_GREEN ANSI_BOLD);
		printf("[%p] Cell constructed.\n", cell);
		printf(ANSI_OFF);
	}

	return cell;
}

void Cell_destruct(Cell *cell)
{
	if (cell == NULL) return;

	free(cell);

	if (QUEUE_DEBUG_PRINT)
	{
		printf(ANSI_RED ANSI_BOLD);
		printf("[%p] Cell destructed.\n", cell);
		printf(ANSI_OFF);
	}
}

void Cell_destruct_recursive(Cell *cell)
{
	if (cell == NULL) return;

	Cell_destruct_recursive(cell->next);
	Cell_destruct(cell);
}

Queue *Queue_construct()
{
	Queue *queue = (Queue *)malloc(sizeof(Queue));

	queue->first = NULL;
	queue->last  = NULL;

	if (QUEUE_DEBUG_PRINT)
	{
		printf(ANSI_GREEN ANSI_BOLD);
		printf("[%p] Queue constructed.\n", queue);
		printf(ANSI_OFF);
	}

	return queue;
}

void Queue_destruct(Queue *queue)
{
	if (queue == NULL) return;

	Cell_destruct_recursive(queue->first);
	free(queue);

	if (QUEUE_DEBUG_PRINT)
	{
		printf(ANSI_RED ANSI_BOLD);
		printf("[%p] Queue destructed.\n", queue);
		printf(ANSI_OFF);
	}
}

void Queue_print(Queue *queue)
{
	if (queue == NULL) return;

	Cell *current = queue->first;

	printf("First -> Last { ");
	if (current != NULL)
	{
		printf("%i", current->value);
		current = current->next;
		while (current != NULL)
		{
			printf(", %i", current->value);
			current = current->next;
		}
	}
	printf(" }\n");
}

void Queue_clear(Queue *queue)
{
	if (queue == NULL) return;
}

void Queue_enqueue(Queue *queue, int value)
{
	if (queue == NULL) return;

	Cell **current = &queue->first;

	while (*current != NULL)
	{
		current = &(*current)->next;
	}

	*current = Cell_construct(value);

	queue->last = *current;
}

void Queue_enqueue_optimized(Queue *queue, int value)
{
	if (queue == NULL) return;

	Cell *new = Cell_construct(value);
	if (queue->last == NULL)
	{
		queue->first = new;
	}
	else
	{
		queue->last->next = new;
	}

	queue->last = new;
}

dequeue_result Queue_dequeue(Queue *queue)
{
	dequeue_result result = {
		0,
		false
	};

	if (queue != NULL && queue->first != NULL)
	{
		result.value = queue->first->value;
		result.valid = true;

		Cell *to_remove = queue->first;
		queue->first = to_remove->next;
		Cell_destruct(to_remove);
		if (queue->first == NULL)
		{
			queue->last = NULL;
		}
	}

	return result;
}
