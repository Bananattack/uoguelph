/* === TESTING LIST TYPE === */

#include <stdlib.h>
#include <stdio.h>
#include "adt/Stack.h"


#define BUFSIZE 81


struct etype
{
	int key;
	char str[10];
};


int main()
{
	Stack *tst;
	int opt, misc;
	char dummy[BUFSIZE];
	struct etype *el, *rel;

	if (!(tst = stackCreate()))
		exit(-1);

	while(1)
	{
		printf("\n\n\nStack (<ctrl-C> to quit):\n");
		printf("1 - stackClear()\n");
		printf("2 - stackPush()\n");
		printf("3 - stackPop()\n");
		printf("4 - stackIsEmpty()\n");
		printf("5 - stackLength()\n");

		printf("\n");
		printf("CHOICE: ");
		gets(dummy);
		sscanf(dummy,"%d",&opt);

		switch(opt)
		{
			case 1:
				if ((misc = stackClear(tst)) < 0)
				{
					fprintf(stderr,"stackClear error\n");
					exit(-1);
				}
				else
					printf("\nStack of size %d cleared\n",misc);
				break;
			case 2:
				el = malloc(sizeof(struct etype));
				printf("\nKEY(int)       : ");
				gets(dummy);
				sscanf(dummy,"%d", &el->key);
				printf("STRING(char *) : ");
				gets(el->str);
				stackPush(tst,el);
				break;
			case 3:
				if (!(rel = (struct etype *)stackPop(tst)))
					fprintf(stderr,"\n>>> Stack is empty\n");
				else
					printf("\nELEMENT : Key: %d\n          Str: %s\n",rel->key, rel->str);
				break;
			case 4:
				printf("\nStack is %s\n",(listIsEmpty(tst) ? "empty" : "non-empty"));
				break;
			case 5:
				printf("\nSize of stack: %d\n",listLength(tst));
				break;
			default:
				printf("\n>>> ILLEGAL SELECTION\n");
				break;
		}

		printf("\nDone.\nHit <enter>...\n");
		gets(dummy);
	}

	stackDelete(tst);

}
