#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "adt/BinaryTree.h"
#include "adt/AVLTree.h"

typedef struct LetterCount_t
{
	char letter;
	int occurrences;
} LetterCount;

bool IsAlpha(char c)
{
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

LetterCount* LetterCountNew(char letter)
{
	LetterCount* this = malloc(sizeof(LetterCount));
	if(this != NULL)
	{
		this->letter = letter;
		this->occurrences = 1;
	}
}

void LetterCountFree(LetterCount* this)
{
	if(this != NULL)
	{
		free(this);
	}
}

void LetterCountIncrease(LetterCount* this)
{
	if(this != NULL)
	{
		this->occurrences++;
	}
}

int LetterCountCompare(void* a, void* b)
{
	LetterCount* ls = a;
	LetterCount* rs = b;
	if(ls != NULL && rs != NULL)
	{
		if(ls->letter < rs->letter)
		{
			return -1;
		}
		if(ls->letter == rs->letter)
		{
			return 0;
		}
		if(ls->letter > rs->letter)
		{
			return 1;
		}
	}
	/* Undefined comparison if one of the pointers is NULL, we'll return 0. */
	return 0;
}

void LetterCountPrint(void* data, FILE* f)
{
	LetterCount* this = data;
	if(this != NULL)
	{
		fprintf(f, "Letter %c = %d\n", this->letter, this->occurrences);
	}
	else
	{
		fprintf(f, "Not found\n");
	}
}

#define BUFFER_SIZE 4096

#define TREE_TYPE_BINARY 1
#define TREE_TYPE_AVL 2

int main()
{
	FILE* f = NULL;
	char buffer[BUFFER_SIZE];
	LetterCount* lc;
	LetterCount* result = NULL;
	int i;
	int mode = 0;
	BinaryTree* bin_tree;
	AVLTree* avl_tree;

	while (mode != TREE_TYPE_BINARY && mode != TREE_TYPE_AVL)
	{
		printf("Please enter your tree type (1 = Binary, 2 = AVL): ");
		fgets(buffer, BUFFER_SIZE, stdin);
		sscanf(buffer, "%d", &mode);
	}

	if(mode == TREE_TYPE_BINARY)
	{
		bin_tree = BinaryTreeNew(LetterCountCompare, LetterCountPrint);
	}
	if(mode == TREE_TYPE_AVL)
	{
		avl_tree = AVLTreeNew(LetterCountCompare, LetterCountPrint);
	}

	while(!f)
	{
		printf("\nEnter the filename (or type \"quit\" to exit): ");

		fgets(buffer, BUFFER_SIZE, stdin);
		/* Remove newline */
		buffer[strlen(buffer) - 1] = '\0';

		if (strcmp(buffer, "quit") == 0)
		{
			break;
		}

		f = fopen(buffer,"r");
		if (!f)
		{
			fprintf(stderr,"Can't open file named '%s'", buffer);
			continue;
		}

		while(fgets(buffer, BUFFER_SIZE, f))
		{
			for(i = 0; i < strlen(buffer); i++)
			{
				if(IsAlpha(buffer[i]))
				{
					buffer[i] = tolower(buffer[i]);
					lc = LetterCountNew(buffer[i]);

					if(mode == TREE_TYPE_BINARY)
					{
						result = BinaryTreeRetrieve(bin_tree, lc);
					}
					if(mode == TREE_TYPE_AVL)
					{
						result = AVLTreeRetrieve(avl_tree, lc);
					}

					if(result)
					{
						LetterCountFree(lc);
						LetterCountIncrease(result);
					}
					else
					{
						if(mode == TREE_TYPE_BINARY)
						{
							BinaryTreeInsert(bin_tree, lc);
						}
						if(mode == TREE_TYPE_AVL)
						{
							AVLTreeInsert(avl_tree, lc);
						}
					}
				}
			}	
		}
	}
	if(f)
	{
		if(mode == TREE_TYPE_BINARY)
		{
			BinaryTreePrint(bin_tree, stdout);
		}
		if(mode == TREE_TYPE_AVL)
		{
			AVLTreePrint(avl_tree, stdout);
		}
	}
	if(mode == TREE_TYPE_BINARY)
	{
		BinaryTreeFree(bin_tree);
	}
	if(mode == TREE_TYPE_AVL)
	{
		AVLTreeFree(avl_tree);
	}
	return 0;
}
