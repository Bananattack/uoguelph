/*
	AVLTree.h - AVL Tree ADT prototypes/data declarations
	Mostly wraps the binary tree interface, save for insertion.

	Author: Andrew G. Crowell
*/

#ifndef AVL_TREE_H
#define AVL_TREE_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "BinaryTree.h"

typedef BinaryTree AVLTree;

#define AVLTreeNew BinaryTreeNew
#define AVLTreeFree BinaryTreeFree
#define AVLTreeRetrieve BinaryTreeRetrieve
#define AVLTreePrint BinaryTreePrint

void AVLTreeInsert(AVLTree* this, void* data);

#endif

