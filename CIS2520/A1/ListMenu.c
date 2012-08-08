/* === TESTING LIST TYPE === */

#include <stdlib.h>
#include <stdio.h>
#include "adt/CList.h"


#define BUFSIZE 81


struct etype
{
	int key;
	char str[10];
};


int main()
{
	List *tst;
	int opt, misc, savepos;
	char dummy[BUFSIZE];
	struct etype *el, *rel;

	if (!(tst = listCreate()))
		exit(-1);

	while(1)
	{
		printf("\n\n\nList (<ctrl-C> to quit):\n");
		printf("1  - listClear()\n");
		printf("2  - listHead()\n");
		printf("3  - listTail()\n");
		printf("4  - listPrev()\n");
		printf("5  - listNext()\n");
		printf("6  - listMoveToNth()\n");
		printf("7  - listAddAfter()\n");
		printf("8  - listAddBefore()\n");
		printf("9  - listGetCurrent()\n");
		printf("10 - listSetCurrent()\n");
		printf("11 - listDelCurrent()\n");
		printf("12 - listIsEmpty()\n");
		printf("13 - listLength()\n");
		printf("14 - listPosition()\n");

		printf("\n");
		printf("CHOICE: ");
		gets(dummy);
		sscanf(dummy,"%d",&opt);

		switch(opt)
		{
			case 1:
				if ((misc = listClear(tst)) < 0)
				{
					fprintf(stderr,"listClear error\n");
					exit(-1);
				}
				else
					printf("\nList of size %d cleared\n",misc);
				break;
			case 2:
				if (!listHead(tst))
					fprintf(stderr,"\n>>> NO MOVEMENT - List is empty\n");
				break;
			case 3:
				if (!listTail(tst))
					fprintf(stderr,"\n>>> NO MOVEMENT - List is empty\n");
				break;
			case 4:
				if (!listPrev(tst))
					fprintf(stderr,"\n>>> List empty, or already at beginning\n");
				break;
			case 5:
				if (!listNext(tst))
					fprintf(stderr,"\n>>> List empty, or already at end\n");
				break;
			case 6:
				printf("\nMove to (n > 0): ");
				gets(dummy);
				sscanf(dummy,"%d",&misc);
				if (!listMoveToNth(tst,misc))
					fprintf(stderr,"\n>>> List empty, or element %d out of range\n",misc);
				break;
			case 7:
				el = malloc(sizeof(struct etype));
				printf("\nKEY(int)       : ");
				gets(dummy);
				sscanf(dummy,"%d", &el->key);
				printf("STRING(char *) : ");
				gets(el->str);
				if (!listAddAfter(tst,el))
				{
					fprintf(stderr,"listAddAfter error\n");
					exit(-1);
				}
				break;
			case 8:
				el = malloc(sizeof(struct etype));
				printf("\nKEY(int)       : ");
				gets(dummy);
				sscanf(dummy,"%d", &el->key);
				printf("STRING(char *) : ");
				gets(el->str);
				if (!listAddBefore(tst,el))
				{
					fprintf(stderr,"listAddBefore error\n");
					exit(-1);
				}
				break;
			case 9:
				if (!(rel = (struct etype *)listGetCurrent(tst)))
					fprintf(stderr,"\n>>> List is empty\n");
				else
					printf("\nELEMENT : Key: %d\n          Str: %s\n",rel->key, rel->str);
				break;
			case 10:
				el = malloc(sizeof(struct etype));
				printf("\nKEY(int)       : ");
				gets(dummy);
				sscanf(dummy,"%d", &el->key);
				printf("STRING(char *) : ");
				gets(el->str);
				if (!(rel = listSetCurrent(tst,el)))
				{
					fprintf(stderr,"listSetCurrent error\n");
					exit(-1);
				}
				free(rel);
				break;
			case 11:
				if (!(rel = (struct etype *)listDelCurrent(tst)))
					fprintf(stderr,"\n>>> NO DELETION : List is empty\n");
				else
				{
					printf("\nELEMENT : Key: %d\n          Str: %s\n",rel->key, rel->str);
					free(rel);
				}
				break;
			case 12:
				printf("\nList is %s\n",(listIsEmpty(tst) ? "empty" : "non-empty"));
				break;
			case 13:
				printf("\nSize of list : %d\n",listLength(tst));
				break;
			case 14:
				printf("\nPosition : %d\n",listPosition(tst));
				break;
			default:
				printf("\n>>> ILLEGAL SELECTION\n");
				break;
		}
		printf("\n============\n\n");
		printf("NEW (CURRENT) STATE:\n");

		savepos = listPosition(tst);

		listHead(tst);
		if (listIsEmpty(tst))
			printf("List is empty\n");
		else
		{
			do
			{
				el = listGetCurrent(tst);
				if (savepos == listPosition(tst))
				{
					printf("*>");
					printf("[%d|%s]",el->key, el-> str);
					printf("<*  ");
				}
				else
					printf("[%d|%s]  ",el->key, el-> str);
			} while(listNext(tst));
			printf("\n");
		}

		listMoveToNth(tst,savepos);

		printf("\nHit <enter>...\n");
		gets(dummy);
	}

	listDelete(tst);

}
