#include <stdio.h>
#include <stdlib.h>

struct data
{
	int number;
	struct data *next;
	struct data *prev;
};
typedef struct data datatype;

datatype* addElement(int value, datatype *root)
{
	datatype *temp;
	/* Allocate structure and store value */
	temp = malloc(sizeof(datatype));
	temp->number = value;
	temp->next = NULL;
	temp->prev = NULL;

	/* Add new element to list, adjust pointers, test for NULL root */
	if (root == NULL)
	{
		root = temp;
	}
	else
	{
		if (root->next != NULL)
		{
			root->next->prev = temp;
		}
		temp->next = root->next;
		root->next = temp;
		temp->prev = root;
	}

	/* Return new root of list */
	return root;
}

void printList(datatype *root)
{
	datatype *temp, *last;

	/* print the list by steping though it in both directions
		using both next and prev pointers */

	/* step through the list starting with the root, use next pointers */
	printf("List ordered from root\n");
	temp = root;
	while (temp != NULL)
	{
		printf("%d\n", temp->number);
		last = temp;
		temp = temp->next;
	}

	/* Step through list starting with last element, use prev pointers */
	printf("List ordered from tail\n");
	temp = last;
	while (temp != NULL)
	{
		printf("%d\n", temp->number);
		temp = temp->prev;
	}
}

void freeList(datatype *root)
{
	datatype *temp;
	while (root != NULL)
	{
		temp = root;
		root = temp->next;
		free(temp);
	}
}



int main()
{
	datatype *root = NULL;

	/* Create a linked list of five records */
	root = addElement(7, root);
	root = addElement(3, root);
	root = addElement(5, root);
	root = addElement(10, root);
	root = addElement(1, root);

	printList(root);

	freeList(root);
	return 0;
}
