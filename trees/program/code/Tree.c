#include "Tree.h"

#include "ANSI.h"
#include "Stack.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

bool TREE_DEBUG_MODE = false;
bool TREE_DUMMY_ADD  = false;

Node *Node_construct(int value)
{
	Node *node = (Node *)malloc(sizeof(Node));
	node->value = value;
	node->left  = NULL;
	node->right = NULL;

	if (TREE_DEBUG_MODE)
	{
		printf(ANSI_BOLD ANSI_GREEN "[%p] Node constructed.\n" ANSI_OFF, node);
	}

	return node;
}

void Node_destruct(Node *node)
{
	if (node == NULL) return;

	Node_destruct(node->left);
	Node_destruct(node->right);
	free(node);

	if (TREE_DEBUG_MODE)
	{
		printf(ANSI_BOLD ANSI_RED "[%p] Node destructed.\n" ANSI_OFF, node);
	}
}

void Node_add(Node **node, int value)
{
	if (node == NULL) return;

	if (*node == NULL)
	{
		if (TREE_DUMMY_ADD)
		{
			Node_destruct(Node_construct(value));
		}
		else
		{
			*node = Node_construct(value);
		}
	}
	else
	{
		if (value < (*node)->value)
		{
			Node_add(&(*node)->left, value);
		}
		else if (value > (*node)->value)
		{
			Node_add(&(*node)->right, value);
		}
	}
}

void Node_print(Node *node)
{
	if (node == NULL) return;

	Node_print(node->left);
	printf("%i\n", node->value);
	Node_print(node->right);
}

Tree *Tree_construct()
{
	Tree *tree = (Tree *)malloc(sizeof(Tree));
	tree->root = NULL;

	if (TREE_DEBUG_MODE)
	{
		printf(ANSI_BOLD ANSI_GREEN "[%p] Tree constructed.\n" ANSI_OFF, tree);
	}

	return tree;
}

void Tree_destruct(Tree *tree)
{
	if (tree == NULL) return;

	Node_destruct(tree->root);

	if (TREE_DEBUG_MODE)
	{
		printf(ANSI_BOLD ANSI_RED "[%p] Tree destructed.\n" ANSI_OFF, tree);
	}
}

void Tree_add(Tree *tree, int value)
{
	if (tree == NULL) return;

	Node **current = &tree->root;

	while (*current != NULL)
	{
		if (value < (*current)->value)
		{
			current = &(*current)->left;
		}
		else if (value > (*current)->value)
		{
			current = &(*current)->right;
		}
		else // value already exists
		{
			return;
		}
	}

	if (TREE_DUMMY_ADD)
	{
		Node_destruct(Node_construct(value));
	}
	else
	{
		*current = Node_construct(value);
	}
}

void Tree_add_recursive(Tree *tree, int value)
{
	if (tree == NULL) return;
	Node_add(&tree->root, value);
}

bool Tree_lookup(Tree *tree, int value)
{
	if (tree == NULL) return false;

	Node **current = &tree->root;

	while (*current != NULL)
	{
		if (value < (*current)->value)
		{
			current = &(*current)->left;
		}
		else if (value > (*current)->value)
		{
			current = &(*current)->right;
		}
		else
		{
			return true;
		}
	}

	return false;
}

void Tree_print(Tree *tree)
{
	if (tree == NULL) return;
	Node_print(tree->root);
}

void Tree_print_with_explicit_stack(Tree *tree)
{
	if (tree == NULL || tree->root == NULL) return;

	Stack *stack = Stack_construct();
	Node *current = tree->root;

	// moving to the leftmost node
	while (current->left != NULL)
	{
		Stack_push(stack, current);
		current = current->left;
	}

	while (current != NULL)
	{
		printf("%i\n", current->value);

		if (current->right != NULL)
		{
			current = current->right;
			while (current->left != NULL)
			{
				Stack_push(stack, current);
				current = current->left;
			}
		}
		else
		{
			pop_result result = Stack_pop(stack);
			current = result.valid ? result.value : NULL;
		}
	}

	Stack_destruct(stack);
}
