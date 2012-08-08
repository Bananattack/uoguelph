/*
	BinaryTree.c - Binary Tree ADT implementation

	Author: Andrew G. Crowell
*/
#include "BinaryTree.h"

/*
	BinaryTreeNew
	Creates a new empty binary tree.
	PRE:	None
	POST:	The new binary tree is allocated and returned.
	ERRORS:	NULL - memory allocation failed.
*/
BinaryTree* BinaryTreeNew(int (*comparator)(void* ls, void* rs), void (*print)(void* data, FILE* f))
{
	BinaryTree* this = malloc(sizeof(BinaryTree));
	if(this != NULL)
	{
		this->comparator = comparator;
		this->print = print;
		this->root = NULL;
	}
	return this;
}

/*
	BinaryTreeFree
	Frees a binary tree from memroy.
	PRE:	Binary tree was not already freed, pointer is valid binary tree reference.
	POST:	The tree reference is destroyed. Any further tree operations are undefined.
	ERRORS:	None
*/
void BinaryTreeFree(BinaryTree* this)
{
	if(this != NULL)
	{
		BinaryTreeNodeFree(this->root);
		free(this);
	}
}

/*
	BinaryTreeInsert
	Inserts an item into a binary tree.
	PRE:	None
	POST:	The data is placed in the tree, stored according
		to a compared order, where left branches are "less
		than" their parents and right branches are "greater
		than" their parents.
	ERRORS:	None
		(but if your tree grows unreasonably large,
		there's a chance an item won't be added,
		because it can fail if you're out of
		memory. Highly unlikely to occur in practical use though)
*/
void BinaryTreeInsert(BinaryTree* this, void* data)
{
	int comparison = 0;
	BinaryTreeNode* last_node = NULL;
	BinaryTreeNode* node = NULL;
	if(this == NULL)
	{
		return;
	}
	if(this->root == NULL)
	{
		this->root = BinaryTreeNodeNew();
		BinaryTreeNodeSetData(this->root, data);
		return;
	}
	node = this->root;
	while(node != NULL)
	{
		comparison = this->comparator(data, BinaryTreeNodeGetData(node));
		last_node = node;
		if(comparison < 0)
		{
			node = BinaryTreeNodeGetLeft(node);
		}
		else
		{
			node = BinaryTreeNodeGetRight(node);
		}
	}
	
	node = BinaryTreeNodeNew();
	BinaryTreeNodeSetData(node, data);
	if(comparison < 0)
	{
		BinaryTreeNodeSetLeft(last_node, node);
	}
	else
	{
		BinaryTreeNodeSetRight(last_node, node);
	}
}

/*
	BinaryTreeRetrieve
	Looks for an "equal" item in the tree, according
	to the binary tree's comparison callback.
	PRE:	None
	POST:	Returns an item that is considered equal to the data passed in,
		or NULL if there was nothing that matched.
	ERRORS:	NULL - item not found
*/
void* BinaryTreeRetrieve(BinaryTree* this, void* data)
{
	int comparison = 0;
	BinaryTreeNode* last_node = NULL;
	BinaryTreeNode* node = NULL;
	if(this == NULL || this->root == NULL)
	{
		return NULL;
	}
	node = this->root;
	while(node != NULL)
	{
		comparison = this->comparator(data, BinaryTreeNodeGetData(node));
		last_node = node;
		if(comparison < 0)
		{
			last_node = node;
			node = BinaryTreeNodeGetLeft(node);
		}
		else if(comparison == 0)
		{
			return BinaryTreeNodeGetData(node);	
		}
		else
		{
			node = BinaryTreeNodeGetRight(node);
		}
	}

	return NULL;
}

/*
	BinaryTreePrint
	Writes a string representation of the binary tree to an output stream.
	PRE:	File handle is valid
	POST:	The text representation is written to the file.
	ERRORS:	None
*/
void BinaryTreePrint(BinaryTree* this, FILE* f)
{
	if(this != NULL)
	{
		BinaryTreeNodePrint(this->root, this->print, f);
		fprintf(f, "Tree height = %d\n", BinaryTreeNodeGetHeight(this->root));
	}
}

int BinaryTreeFindShortestPath(BinaryTree* this)
{
	return BinaryTreeNodeFindShortestPath(this->root, 0);
}

int BinaryTreeCountInternalNodes(BinaryTree* this)
{
	return BinaryTreeNodeCountInternalNodes(this->root);
}

int BinaryTreeCountInternalLength(BinaryTree* this)
{
	return BinaryTreeNodeCountInternalLength(this->root, 0);
}

