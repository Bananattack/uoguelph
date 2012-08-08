/*
	AVLTree.c - AVL Tree ADT implementation

	Author: Andrew G. Crowell
*/
#include "AVLTree.h"

/*
	AVLTreeInsert
	Inserts an item into an AVL tree.
	PRE:	None
	POST:	The data is placed in the tree, stored according
		to a compared order, where left branches are "less
		than" their parents and right branches are "greater
		than" their parents. Additional steps are performed
		in order to keep the tree near-perfectly balanced.
	ERRORS:	None
*/
void AVLTreeInsert(AVLTree* this, void* data)
{
	bool taller = true;
	this->root = BinaryTreeNodeAVLInsert(this->root, data, &taller, this->comparator);
}
