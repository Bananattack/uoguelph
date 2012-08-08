/*
	BinaryTreeNode.c - Binary Tree Node ADT implementation

	Author: Andrew G. Crowell
*/
#include "BinaryTreeNode.h"

/*
	BinaryTreeNodeNew
	Creates a new tree node that is initially holding nothing,
	associated with no other nodes, and balanced (for AVL nodes).
	PRE:	None
	POST:	
	ERRORS:	NULL - memory allocation failed.
*/
BinaryTreeNode* BinaryTreeNodeNew()
{
	BinaryTreeNode* this = malloc(sizeof(BinaryTreeNode));
	if(this != NULL)
	{
		this->parent = NULL;
		this->left = NULL;
		this->right = NULL;
		this->data = NULL;
		this->balance = EQUAL;
	}
	return this;
}

/*
	BinaryTreeNodeFree
	Frees a node from memroy.
	PRE:	Node was not already freed, pointer is valid node reference.
	POST:	The node reference is destroyed. Any further node operations are undefined.
	ERRORS:	None
*/
void BinaryTreeNodeFree(BinaryTreeNode* this)
{
	if(this != NULL)
	{
		BinaryTreeNodeFree(this->left);
		BinaryTreeNodeFree(this->right);
		free(this);
	}
}

/*
	BinaryTreeNodeSetLeft
	Sets the node's left child node.
	PRE:	None
	POST:	Left child is changed.
	ERRORS:	None
*/
void BinaryTreeNodeSetLeft(BinaryTreeNode* this, BinaryTreeNode* node)
{
	if(this != NULL)
	{
		this->left = node;
	}
}

/*
	BinaryTreeNodeSetLeft
	Sets the node's right child node.
	PRE:	None
	POST:	Right child is changed.
	ERRORS:	None
*/
void BinaryTreeNodeSetRight(BinaryTreeNode* this, BinaryTreeNode* node)
{
	if(this != NULL)
	{
		this->right = node;
	}
}

/*
	BinaryTreeNodeSetData
	Sets the node's data pointer
	PRE:	None
	POST:	Node stores the data passed.
	ERRORS:	None
*/
void BinaryTreeNodeSetData(BinaryTreeNode* this, void* data)
{
	if(this != NULL)
	{
		this->data = data;
	}
}

/*
	BinaryTreeNodeGetLeft
	Gets the node's left child node.
	PRE:	None
	POST:	Reference to the left child is returned to its caller.
	ERRORS:	None
*/
BinaryTreeNode* BinaryTreeNodeGetLeft(BinaryTreeNode* this)
{
	if(this != NULL)
	{
		return this->left;
	}
	return NULL;
}

/*
	BinaryTreeNodeGetRight
	Gets the node's right child node.
	PRE:	None
	POST:	Reference to the right child is returned to its caller.
	ERRORS:	None
*/
BinaryTreeNode* BinaryTreeNodeGetRight(BinaryTreeNode* this)
{
	if(this != NULL)
	{
		return this->right;
	}
	return NULL;
}

/*
	BinaryTreeNodeGetData
	Gets the node's associated data.
	PRE:	None
	POST:	Reference to the data held by the node.
	ERRORS:	None
*/
void* BinaryTreeNodeGetData(BinaryTreeNode* this)
{
	if(this != NULL)
	{
		return this->data;
	}
	return NULL;
}

/*
	BinaryTreeNodePrint
	Does an in-order traversal of the tree nodes,
	printing all tree contents to a file
	PRE:	None
	POST:	Node and its children printed to file
	ERRORS:	None
*/
void BinaryTreeNodePrint(BinaryTreeNode* this, void (*print)(void* data, FILE* f), FILE* f)
{
	_BinaryTreeNodePrint(this, print, 1, f);
}

/*
	_BinaryTreeNodePrint
	Does an in-order traversal of the tree nodes,
	printing all tree contents to a file (with initial)
	PRE:	None
	POST:	Node and its children printed to file
	ERRORS:	None
*/
void _BinaryTreeNodePrint(BinaryTreeNode* this, void (*print)(void* data, FILE* f), int count, FILE* f)
{
	if(this != NULL)
	{
		_BinaryTreeNodePrint(this->left, print, count + 1, f);
		fprintf(f, "%d: ", count); print(this->data, f);
		_BinaryTreeNodePrint(this->right, print, count + 1, f);
	}
}

/*
	BinaryTreeNodeGetHeight
	Gets the height of the tree which starts at this node,
	which is the maximum of its right and left subtree's lengths
	plus one.
	PRE:	None
	POST:	Returns the height of the tree which starts at this node.
	ERRORS:	0 - if the node is NULL.
*/
int BinaryTreeNodeGetHeight(BinaryTreeNode* this)
{
	int lh, rh;
	if(this != NULL)
	{
		lh = BinaryTreeNodeGetHeight(this->left);
		rh = BinaryTreeNodeGetHeight(this->right);
		return BINARY_TREE_NODE_MAX(lh, rh) + 1;
	}
	return 0;
}

/*
	BinaryTreeNodeSetBalance
	Sets the node's balance factor
	PRE:	None
	POST:	Changes the node's balance.
	ERRORS:	None
*/
void BinaryTreeNodeSetBalance(BinaryTreeNode* this, Balance balance)
{
	this->balance = balance;
}

/*
	BinaryTreeNodeGetBalance
	Gets the node's balance factor
	PRE:	None
	POST:	Returns the node's balance.
	ERRORS:	None
*/
Balance BinaryTreeNodeGetBalance(BinaryTreeNode* this)
{
	return this->balance;
}

/*
	BinaryTreeNodeAVLInsert
	Inserts a node into a tree starting at this node,
	and using AVL methods, balances itself.
	PRE:	None
	POST:	Returns the new "root" node of the tree previously starting at this node
		after insertion of a new data node.
	ERRORS:	None
*/
BinaryTreeNode* BinaryTreeNodeAVLInsert(BinaryTreeNode* this, void* data, bool* taller, int (*comparator)(void* ls, void* rs))
{
	BinaryTreeNode* tmp = NULL;

	if(this == NULL)
	{
		*taller = true;
		tmp = BinaryTreeNodeNew();
		BinaryTreeNodeSetData(tmp, data);
		return tmp;
	}
	else if(comparator(data, BinaryTreeNodeGetData(this)) < 0)
	{
		BinaryTreeNodeSetLeft(this, BinaryTreeNodeAVLInsert(BinaryTreeNodeGetLeft(this), data, taller, comparator));
		if (*taller) /* left subtree is taller */
		{
			switch(BinaryTreeNodeGetBalance(this))
			{
				case LHIGH: /* node was already left high */
					this = _BinaryTreeNodeBalanceLeft(this);
					*taller = true;
					break;
				case EQUAL: /* node is now left high */
					BinaryTreeNodeSetBalance(this, LHIGH);
					break;
				case RHIGH: /* node now has balanced height */
					BinaryTreeNodeSetBalance(this, EQUAL);
					*taller = false;
					break;
			}

		}
	}
	else if(comparator(data, BinaryTreeNodeGetData(this)) > 0)
	{
		BinaryTreeNodeSetRight(this, BinaryTreeNodeAVLInsert(BinaryTreeNodeGetRight(this), data, taller, comparator));
		if (*taller) /* left subtree is taller */
		{
			switch(BinaryTreeNodeGetBalance(this))
			{
				case LHIGH: /* node now has balanced height */
					BinaryTreeNodeSetBalance(this, EQUAL);
					*taller = false;
					break;
				case EQUAL: /* node is now right high */
					BinaryTreeNodeSetBalance(this, RHIGH);
					break;
				case RHIGH: /* node was already right high */
					this = _BinaryTreeNodeBalanceRight(this);
					*taller = true;
					break;
			}

		}
	}
	return this;
}

/*
	_BinaryTreeNodeRotateLeft
	Rearranges this and its left and right children in leftwise rotation
	PRE:	None
	POST:	Returns the new "parent" node after rotation
	ERRORS:	No change - rotation is not allowed.
*/
BinaryTreeNode* _BinaryTreeNodeRotateLeft(BinaryTreeNode* this)
{
	BinaryTreeNode* tmp;
	/* ERROR - can’t rotate an empty subtree */
	if (BinaryTreeNodeGetRight(this) == NULL)
	{
		return this;
	}
	else
	{
		tmp = BinaryTreeNodeGetRight(this);
		BinaryTreeNodeSetRight(this, BinaryTreeNodeGetLeft(tmp));
		BinaryTreeNodeSetLeft(tmp, this);
		return tmp;
	}
}

/*
	_BinaryTreeNodeRotateRight
	Rearranges this and its left and right children in rightwise rotation
	PRE:	None
	POST:	Returns the new "parent" node after rotation
	ERRORS:	No change - rotation is not allowed.
*/
BinaryTreeNode* _BinaryTreeNodeRotateRight(BinaryTreeNode* this)
{
	BinaryTreeNode* tmp;
	/* ERROR - can’t rotate an empty subtree */
	if (BinaryTreeNodeGetLeft(this) == NULL)
	{
		return this;
	}
	else
	{
		tmp = BinaryTreeNodeGetLeft(this);
		BinaryTreeNodeSetLeft(this, BinaryTreeNodeGetRight(tmp));
		BinaryTreeNodeSetRight(tmp, this);
		return tmp;
	}
}

/*
	_BinaryTreeNodeBalanceRight
	Rearranges the tree starting at this node in order to make the
	tree storage more balanced on the right.
	PRE:	None
	POST:	Returns the new "parent" node after rotation
	ERRORS:	ERRORS:	No change - rotation is not allowed.
*/
BinaryTreeNode* _BinaryTreeNodeBalanceRight(BinaryTreeNode* this)
{
	switch(BinaryTreeNodeGetBalance(this))
	{
		case RHIGH: /* Single rotation leftwise */
			BinaryTreeNodeSetBalance(this, EQUAL);
			BinaryTreeNodeSetBalance(BinaryTreeNodeGetRight(this), EQUAL);
			return _BinaryTreeNodeRotateLeft(this);
		case LHIGH: /* Double rotation leftwise */
			switch(BinaryTreeNodeGetBalance(BinaryTreeNodeGetLeft(BinaryTreeNodeGetRight(this))))
			{
				case RHIGH:
					BinaryTreeNodeSetBalance(this, LHIGH);
					BinaryTreeNodeSetBalance(BinaryTreeNodeGetRight(this), EQUAL);
					break;
				case EQUAL:
					BinaryTreeNodeSetBalance(this, EQUAL);
					BinaryTreeNodeSetBalance(BinaryTreeNodeGetRight(this), EQUAL);
					break;
				case LHIGH:
					BinaryTreeNodeSetBalance(this, EQUAL);
					BinaryTreeNodeSetBalance(BinaryTreeNodeGetRight(this), RHIGH);
					break;
			}
			BinaryTreeNodeSetBalance(BinaryTreeNodeGetLeft(BinaryTreeNodeGetRight(this)), EQUAL);
			BinaryTreeNodeSetRight(this, _BinaryTreeNodeRotateRight(BinaryTreeNodeGetRight(this)));
			return _BinaryTreeNodeRotateLeft(this);
		case EQUAL:
			/* ERROR - right subtree cannot have equal balance */
			return this;
	}
	return this;
}

/*
	_BinaryTreeNodeBalanceLeft
	Rearranges the tree starting at this node in order to make the
	tree storage more balanced on the left.
	PRE:	None
	POST:	Returns the new "parent" node after rotation
	ERRORS:	ERRORS:	No change - rotation is not allowed.
*/
BinaryTreeNode* _BinaryTreeNodeBalanceLeft(BinaryTreeNode* this)
{
	switch(BinaryTreeNodeGetBalance(this))
	{
		case LHIGH: /* Single rotation Rightwise */
			BinaryTreeNodeSetBalance(this, EQUAL);
			BinaryTreeNodeSetBalance(BinaryTreeNodeGetLeft(this), EQUAL);
			return _BinaryTreeNodeRotateRight(this);
		case RHIGH: /* Double rotation Rightwise */
			switch(BinaryTreeNodeGetBalance(BinaryTreeNodeGetRight(BinaryTreeNodeGetLeft(this))))
			{
				case LHIGH:
					BinaryTreeNodeSetBalance(this, RHIGH);
					BinaryTreeNodeSetBalance(BinaryTreeNodeGetLeft(this), EQUAL);
					break;
				case EQUAL:
					BinaryTreeNodeSetBalance(this, EQUAL);
					BinaryTreeNodeSetBalance(BinaryTreeNodeGetLeft(this), EQUAL);
					break;
				case RHIGH:
					BinaryTreeNodeSetBalance(this, EQUAL);
					BinaryTreeNodeSetBalance(BinaryTreeNodeGetLeft(this), LHIGH);
					break;
			}
			BinaryTreeNodeSetBalance(BinaryTreeNodeGetRight(BinaryTreeNodeGetLeft(this)), EQUAL);
			BinaryTreeNodeSetLeft(this, _BinaryTreeNodeRotateLeft(BinaryTreeNodeGetLeft(this)));
			return _BinaryTreeNodeRotateRight(this);
		case EQUAL:
			/* ERROR - Left subtree cannot have equal balance */
			return this;
	}
	return this;
}

int BinaryTreeNodeFindShortestPath(BinaryTreeNode* this, int height)
{
	int l, r;
	if(this == NULL || (this->left == NULL && this->right == NULL))
	{
		return height;
	}
	l = BinaryTreeNodeFindShortestPath(this->left, height + 1);
	r = BinaryTreeNodeFindShortestPath(this->right, height + 1);
	if(l < r)
	{
		return l;
	}
	else
	{
		return r;
	}
}


int BinaryTreeNodeCountInternalNodes(BinaryTreeNode* this)
{
	if(this == NULL || (this->left == NULL && this->right == NULL))
	{
		return 0;
	}
	else
	{
		return BinaryTreeNodeCountInternalNodes(this->left) + BinaryTreeNodeCountInternalNodes(this->right) + 1;
	}
}

int BinaryTreeNodeCountInternalLength(BinaryTreeNode* this, int height)
{
	if(this == NULL)
	{
		return 0;
	}
	else
	{
		return BinaryTreeNodeCountInternalLength(this->left, height + 1) + BinaryTreeNodeCountInternalLength(this->right, height + 1) + height;
	}
}
