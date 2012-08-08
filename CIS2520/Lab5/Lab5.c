#include <stdio.h>
#include <stdlib.h>
#include "adt/BinaryTree.h"

typedef struct
{
	int value;
} Thing;


Thing* ThingNew(int value)
{
	Thing* this = malloc(sizeof(Thing));
	this->value = value;
	return this;
}

void ThingFree(Thing* this)
{
	free(this);
}

int ThingCompare(void* lhs, void* rhs)
{
	Thing* ls = lhs;
	Thing* rs = rhs;
	if (ls->value < rs->value)
	{
		return -1;
	}
	else if (ls->value > rs->value)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void ThingPrint(void* this, FILE* f)
{
	fprintf(f, "%d\n", ((Thing*) this)->value);
}

int main()
{
	int i;
	BinaryTree* tree = NULL;

	tree = BinaryTreeNew(ThingCompare, ThingPrint);

	BinaryTreeInsert(tree, ThingNew(4));
	BinaryTreeInsert(tree, ThingNew(2));
	BinaryTreeInsert(tree, ThingNew(5));
	BinaryTreeInsert(tree, ThingNew(0));
	BinaryTreeInsert(tree, ThingNew(3));
	BinaryTreeInsert(tree, ThingNew(6));
	BinaryTreeInsert(tree, ThingNew(7));
	BinaryTreeInsert(tree, ThingNew(-2));

	BinaryTreePrint(tree, stdout);
	printf("Shortest path: %d\n", BinaryTreeFindShortestPath(tree));
	printf("Internal node count: %d\n", BinaryTreeCountInternalNodes(tree));
	printf("Internal length: %d\n", BinaryTreeCountInternalLength(tree));
	BinaryTreeFree(tree);

	return 0;
}

