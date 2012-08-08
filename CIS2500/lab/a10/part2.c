#include <stdio.h>
#include <stdlib.h>

struct data {
   int number;
   struct data *next;
};

typedef struct data datatype;

datatype * addElement(int value, datatype *tail) {
datatype *temp;

	/* allocate structure */
   temp = malloc(sizeof(datatype));
   temp->number = value;
   temp->next = tail;
   tail = temp;

   return(tail);
}

datatype * removeElement(datatype *tail) {
	datatype *temp;
	if (tail == NULL)
	{
		return NULL;
	}
	temp = tail->next;
	free(tail);
	return temp;
}

void printStack(datatype * tail) {
datatype *temp;

	/* step through the stack starting with the tail, use next pointers */
   printf("Stack ordered from tail\n");
   temp = tail;
   while (temp != NULL) {
      printf("%d\n", temp->number);
      temp = temp->next;
   }
   printf("\n");
}

void freeStack(datatype *tail) {
datatype *temp;
   while (tail != NULL) {
      temp = tail;
      tail = temp->next;
      free(temp);
   }
}



int main() {
datatype *tail;

   tail = NULL;

	/* create a stack of five records */
   printf("Add 7 to stack\n");
   tail = addElement(7, tail);
   printStack(tail);

   printf("Add 3 to stack\n");
   tail = addElement(3, tail);
   printStack(tail);

   printf("Add 5 to stack\n");
   tail = addElement(5, tail);
   printStack(tail);

   printf("Remove element from stack\n");
   tail = removeElement(tail);
   printStack(tail);

   printf("Remove element from stack\n");
   tail = removeElement(tail);
   printStack(tail);

   printf("Add 10 to stack\n");
   tail = addElement(10, tail);
   printStack(tail);

   printf("Add 3 to stack\n");
   tail = addElement(3, tail);
   printStack(tail);

   freeStack(tail);

   return(1);
}
