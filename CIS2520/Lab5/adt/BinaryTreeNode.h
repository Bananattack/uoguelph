/*
	BinaryTreeNode.h - Binary Tree Node ADT prototypes/data declarations

	Author: Andrew G. Crowell
*/

#ifndef BINARY_TREE_NODE_H
#define BINARY_TREE_NODE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "List.h"

typedef enum { LHIGH, EQUAL, RHIGH } Balance;

typedef struct BinaryTreeNode_t
{
	struct BinaryTreeNode_t* parent;
	struct BinaryTreeNode_t* left;
	struct BinaryTreeNode_t* right;

	Balance balance;
	void* data;
} BinaryTreeNode;

#define BINARY_TREE_NODE_MAX(a,b) (((a) > (b)) ? (a) : (b))

BinaryTreeNode* BinaryTreeNodeNew();
void BinaryTreeNodeFree(BinaryTreeNode* this);
void BinaryTreeNodeSetLeft(BinaryTreeNode* this, BinaryTreeNode* node);
void BinaryTreeNodeSetRight(BinaryTreeNode* this, BinaryTreeNode* node);
void BinaryTreeNodeSetData(BinaryTreeNode* this, void* data);
BinaryTreeNode* BinaryTreeNodeGetLeft(BinaryTreeNode* this);
BinaryTreeNode* BinaryTreeNodeGetRight(BinaryTreeNode* this);
void* BinaryTreeNodeGetData(BinaryTreeNode* this);
void BinaryTreeNodePrint(BinaryTreeNode* this, void (*print)(void* data, FILE* f), FILE* f);
void _BinaryTreeNodePrint(BinaryTreeNode* this, void (*print)(void* data, FILE* f), int count, FILE* f);
int BinaryTreeNodeGetHeight(BinaryTreeNode* this);
void BinaryTreeNodeSetBalance(BinaryTreeNode* this, Balance balance);
Balance BinaryTreeNodeGetBalance(BinaryTreeNode* this);
BinaryTreeNode* BinaryTreeNodeAVLInsert(BinaryTreeNode* this, void* data, bool* taller, int (*comparator)(void* ls, void* rs));
BinaryTreeNode* _BinaryTreeNodeRotateLeft(BinaryTreeNode* this);
BinaryTreeNode* _BinaryTreeNodeRotateRight(BinaryTreeNode* this);
BinaryTreeNode* _BinaryTreeNodeBalanceLeft(BinaryTreeNode* this);
BinaryTreeNode* _BinaryTreeNodeBalanceRight(BinaryTreeNode* this);
int BinaryTreeNodeFindShortestPath(BinaryTreeNode* this, int count);
int BinaryTreeNodeCountInternalNodes(BinaryTreeNode* this);
int BinaryTreeNodeCountInternalLength(BinaryTreeNode* this, int height);

#endif
