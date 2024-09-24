#include "ANSI.h"
#include "Cell.h"

#include <stdio.h>
#include <stdlib.h>

bool CELL_DEBUG_MODE = false;

int alive_cells = 0;

Cell *Cell_construct(int item)
{
	Cell *cell = (Cell *)malloc(sizeof(Cell));

	cell->item = item;
	cell->next = NULL;

	alive_cells++;

	if (CELL_DEBUG_MODE)
	{
		printf("%s%s", ANSI_GREEN, ANSI_BOLD);
		printf("[%p] Cell constructed. Value: %11i, Alive cells: %11i\n", cell, item, alive_cells);
		printf("%s", ANSI_OFF);
	}

	return cell;
}

Cell *Cell_construct_list(size_t size)
{
	Cell *cell = NULL;

	if (size > 0)
	{
		cell = Cell_construct(0);
		for (size_t i = 1; i < size; i++)
		{
			Cell_add(&cell, i);
		}
	}

	return cell;
}

void Cell_destruct(Cell *cell)
{
	if (cell == NULL) return;

	int item = cell->item;

	free(cell);

	alive_cells--;

	if (CELL_DEBUG_MODE)
	{
		printf("%s%s", ANSI_RED, ANSI_BOLD);
		printf("[%p] Cell destructed.  Value: %11i, Alive cells: %11i\n", cell, item, alive_cells);
		printf("%s", ANSI_OFF);
	}
}

void Cell_destruct_recursive(Cell *cell)
{
	if (cell == NULL) return;
	Cell_destruct_recursive(cell->next);
	Cell_destruct(cell);
}

void Cell_add(Cell **cell, int item)
{
	Cell *new = Cell_construct(item);
	new->next = *cell;
	*cell = new;
}

size_t Cell_size(Cell *cell)
{
	size_t size = 0;

	while (cell != NULL)
	{
		size++;
		cell = cell->next;
	}

	return size;
}

bool Cell_contains(Cell *cell, int item)
{
	while (cell != NULL)
	{
		if (cell->item == item) return true;
		cell = cell->next;
	}
	return false;
}

void Cell_remove(Cell **cell, int item)
{
	if (cell == NULL) return;

	Cell **next = &(*cell)->next;
	if (*next != NULL)
	{
		Cell_remove(next, item);
	}

	if ((*cell)->item == item)
	{
		Cell *tmp = *cell;
		*cell = (*cell)->next;
		Cell_destruct(tmp);
	}
}

void Cell_append(Cell *top, Cell *bottom)
{
	if (top == NULL) return;

	while (top->next != NULL)
	{
		top = top->next;
	}

	top->next = bottom;
}

void Cell_print(Cell *cell)
{
	if (cell == NULL)
	{
		printf("NULL\n");
		return;
	}

	printf("%i -> ", cell->item);
	Cell_print(cell->next);
}
