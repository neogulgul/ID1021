#pragma once

#include <stdbool.h>
#include <stddef.h>

extern bool CELL_DEBUG_MODE;

typedef struct Cell Cell;
struct Cell
{
	int item;
	Cell *next;
};

Cell *Cell_construct(int item);
Cell *Cell_construct_list(size_t size);

void Cell_destruct          (Cell *cell);
void Cell_destruct_recursive(Cell *cell);

void Cell_print(Cell *cell);

// Stuff I had to implement for the assignment
void   Cell_add     (Cell **cell, int item);   // add an element to the start of the list
size_t Cell_size    (Cell  *cell);             // returns the number of elements in the list
bool   Cell_contains(Cell  *cell, int item);   // returns true only if the list contains atleast one instance of a certain item
void   Cell_remove  (Cell **cell, int item);   // removes all instances of a certain item
void   Cell_append  (Cell *top, Cell *bottom); // appends bottom to last cell of top
