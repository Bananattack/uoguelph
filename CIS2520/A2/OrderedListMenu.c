/*
	This is a test program for the ordered list menu. 
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "adt/OrderedList.h"


#define BUFFER_SIZE 81


typedef struct TestElement_t
{
	char str[1024];
} TestElement;

int Compare(void* a, void* b)
{
	TestElement* ls = (TestElement*) a; 
	TestElement* rs = (TestElement*) b;
	return strcmp(ls->str, rs->str);
}

int main()
{
	OrderedList *testList;
	int opt, misc, savepos;
	char buffer[BUFFER_SIZE];
	TestElement *el, *rel;

	if (!(testList = OrderedListCreate(Compare)))
	{
		exit(-1);
	}

	while(1)
	{
		printf("\n\n\nList (<ctrl-C> to quit):\n");
		printf("1  - OrderedListClear()\n");
		printf("2  - OrderedListHead()\n");
		printf("3  - OrderedListTail()\n");
		printf("4  - OrderedListPrevious()\n");
		printf("5  - OrderedListNext()\n");
		printf("6  - OrderedListSetPosition()\n");
		printf("7  - OrderedListInsert()\n");
		printf("8  - OrderedListGetCurrent()\n");
		printf("9 - OrderedListDeleteCurrent()\n");
		printf("10 - OrderedListIsEmpty()\n");
		printf("11 - OrderedListGetLength()\n");
		printf("12 - OrderedListGetPosition()\n");
		printf("13 - OrderedListSearch()\n");

		printf("\n");
		printf("CHOICE: ");
		fgets(buffer, BUFFER_SIZE, stdin);
		sscanf(buffer,"%d",&opt);

		switch(opt)
		{
			case 1:
				if ((misc = OrderedListClear(testList)) < 0)
				{
					fprintf(stderr,"OrderedListClear error\n");
					exit(-1);
				}
				else
				{
					printf("\nList of size %d cleared\n",misc);
				}
				break;
			case 2:
				if (!OrderedListHead(testList))
				{
					fprintf(stderr,"\n>>> NO MOVEMENT - List is empty\n");
				}
				break;
			case 3:
				if (!OrderedListTail(testList))
				{
					fprintf(stderr,"\n>>> NO MOVEMENT - List is empty\n");
				}
				break;
			case 4:
				if (!OrderedListPrevious(testList))
				{
					fprintf(stderr,"\n>>> List empty, or already at beginning\n");
				}
				break;
			case 5:
				if (!OrderedListNext(testList))
				{
					fprintf(stderr,"\n>>> List empty, or already at end\n");
				}
				break;
			case 6:
				printf("\nMove to (n > 0): ");
				fgets(buffer, BUFFER_SIZE, stdin);
				sscanf(buffer,"%d",&misc);
				if (!OrderedListSetPosition(testList,misc))
				{
					fprintf(stderr,"\n>>> List empty, or element %d out of range\n",misc);
				}
				break;
			case 7:
				el = malloc(sizeof(TestElement));
				printf("STRING(char *) : ");
				fgets(el->str, 1024, stdin);
				if (!OrderedListInsert(testList,el))
				{
					fprintf(stderr,"OrderedListInsert error\n");
					exit(-1);
				}
				break;
			case 8:
				if (!(rel = (TestElement *)OrderedListGetCurrent(testList)))
				{
					fprintf(stderr,"\n>>> List is empty\n");
				}
				else
				{
					printf("\nELEMENT : Str: %s\n", rel->str);
				}
				break;
			case 9:
				if (!(rel = (TestElement *)OrderedListDeleteCurrent(testList)))
				{
					fprintf(stderr,"\n>>> NO DELETION : List is empty\n");
				}
				else
				{
					printf("\nELEMENT : Str: %s\n", rel->str);
					free(rel);
				}
				break;
			case 10:
				printf("\nList is %s\n",(OrderedListIsEmpty(testList) ? "empty" : "non-empty"));
				break;
			case 11:
				printf("\nSize of list : %d\n",OrderedListGetLength(testList));
				break;
			case 12:
				printf("\nPosition : %d\n",OrderedListGetPosition(testList));
				break;
			case 13:
				el = malloc(sizeof(TestElement));
				printf("STRING TO FIND (char *): ");
				fgets(el->str, 1024, stdin);
				rel = OrderedListSearch(testList,el);
				free(el);
				if (rel != NULL)
				{
					printf("\nFound!");
					printf("\nELEMENT : Str: %s\n", rel->str);
				}
				else
				{
					printf("\nNot found\n");
				}
				break;
			default:
				printf("\n>>> ILLEGAL SELECTION\n");
				break;
		}
		printf("\n============\n\n");
		printf("NEW (CURRENT) STATE:\n");

		savepos = OrderedListGetPosition(testList);

		OrderedListHead(testList);
		if (OrderedListIsEmpty(testList))
		{
			printf("List is empty\n");
		}
		else
		{
			do
			{
				el = OrderedListGetCurrent(testList);
				if (savepos == OrderedListGetPosition(testList))
				{
					printf("> %s", el-> str);
				}
				else
				{
					printf("  %s",el-> str);
				}
			} while(OrderedListNext(testList));
			printf("\n");
		}

		OrderedListSetPosition(testList,savepos);

		printf("\nHit <enter>...\n");
		fgets(buffer, BUFFER_SIZE, stdin);
	}

	OrderedListDestroy(testList);

}
