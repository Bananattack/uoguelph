/* === TESTING LIST TYPE === */

#include <stdlib.h>
#include <stdio.h>
#include "adt/Queue.h"


#define BUFSIZE 81


struct etype
{
	int key;
	char str[10];
};


int main()
{
	Queue *tst;
	int opt, misc;
	char dummy[BUFSIZE];
	struct etype *el, *rel;

	if (!(tst = queueCreate()))
		exit(-1);

	while(1)
	{
		printf("\n\n\nqueue (<ctrl-C> to quit):\n");
		printf("1 - queueClear()\n");
		printf("2 - queueArrive()\n");
		printf("3 - queueDepart()\n");
		printf("4 - queueIsEmpty()\n");
		printf("5 - queueLength()\n");

		printf("\n");
		printf("CHOICE: ");
		gets(dummy);
		sscanf(dummy,"%d",&opt);

		switch(opt)
		{
			case 1:
				if ((misc = queueClear(tst)) < 0)
				{
					fprintf(stderr,"queueClear error\n");
					exit(-1);
				}
				else
					printf("\nqueue of size %d cleared\n",misc);
				break;
			case 2:
				el = malloc(sizeof(struct etype));
				printf("\nKEY(int)       : ");
				gets(dummy);
				sscanf(dummy,"%d", &el->key);
				printf("STRING(char *) : ");
				gets(el->str);
				queueArrive(tst,el);
				break;
			case 3:
				if (!(rel = (struct etype *)queueDepart(tst)))
					fprintf(stderr,"\n>>> queue is empty\n");
				else
					printf("\nELEMENT : Key: %d\n          Str: %s\n",rel->key, rel->str);
				break;
			case 4:
				printf("\nqueue is %s\n",(listIsEmpty(tst) ? "empty" : "non-empty"));
				break;
			case 5:
				printf("\nSize of queue: %d\n",listLength(tst));
				break;
			default:
				printf("\n>>> ILLEGAL SELECTION\n");
				break;
		}

		printf("\nDone.\nHit <enter>...\n");
		gets(dummy);
	}

	queueDelete(tst);

}
