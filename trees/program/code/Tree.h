#pragma once

#include <stdbool.h>
#include <stddef.h>

extern bool TREE_DEBUG_MODE;
extern bool TREE_DUMMY_ADD;

typedef struct Node Node;
typedef struct Tree Tree;

struct Node
{
	int value;
	Node *left;
	Node *right;
};

struct Tree
{
	Node *root;
};

Node *Node_construct(int value);
void  Node_destruct (Node  *node);
void  Node_add      (Node **node, int value);
void  Node_print    (Node  *node);

Tree *Tree_construct                ();
void  Tree_destruct                 (Tree *tree);
void  Tree_print                    (Tree *tree);
void  Tree_add                      (Tree *tree, int value);
void  Tree_add_recursive            (Tree *tree, int value);
bool  Tree_lookup                   (Tree *tree, int value);
void  Tree_print                    (Tree *tree);
void  Tree_print_with_explicit_stack(Tree *tree);
