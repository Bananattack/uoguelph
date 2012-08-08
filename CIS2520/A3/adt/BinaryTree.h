/*
	BinaryTree.h - Binary Tree ADT prototypes/data declarations

	Author: Andrew G. Crowell
*/

#ifndef BINARY_TREE_H
#define BINARY_TREE_H
#include <stdio.h>
#include <stdlib.h>
#include "BinaryTreeNode.h"

typedef struct BinaryTree_t
{
	int (*comparator)(void* ls, void* rs);
	void (*print)(void* data, FILE* f);
	BinaryTreeNode* root;
} BinaryTree;

BinaryTree* BinaryTreeNew(int (*comparator)(void* ls, void* rs), void (*print)(void* data, FILE* f));
void BinaryTreeFree(BinaryTree* this);
void BinaryTreeInsert(BinaryTree* this, void* data);
void* BinaryTreeRetrieve(BinaryTree* this, void* data);
void BinaryTreePrint(BinaryTree* this, FILE* f);
#endif
