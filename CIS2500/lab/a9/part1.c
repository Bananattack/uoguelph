#include <stdio.h>
#include <stdlib.h>

struct data
{
	int number;
	struct data *next;
};
typedef struct data datatype;

int main()
{
	datatype *root = NULL;
	datatype *temp = NULL;
	int i;

	/* Create a linked list of five records */
	for(i = 0; i < 5; i++)
	{

		temp = malloc(sizeof(datatype));
		temp->number = i;
		temp->next = root;
		root = temp;
	}


	/* Step through the list starting with the head. */
	temp = root;
	while (temp != NULL)
	{
		printf("%d\n", temp->number);
		temp = temp->next;
	}

	return 0;
}
