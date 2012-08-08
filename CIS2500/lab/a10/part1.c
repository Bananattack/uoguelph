#include <stdio.h>
#include <stdlib.h>

struct data {
   int number;
   struct data *next;
};

typedef struct data datatype;

datatype * addElement(int value, datatype *tail) {
datatype *temp, *head;

	/* allocate structure */
   temp = malloc(sizeof(datatype));
   temp->number = value;
   temp->next = NULL;

	/* find head of queue */
   head = tail;
   if (head != NULL) 
      while(head->next != NULL)
         head=head->next;

	/* add new element to head of queue */
   if (head != NULL)
      head->next = temp;
   else
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

void printQueue(datatype * tail) {
datatype *temp;

	/* step through the queue starting with the tail, use next pointers */
   printf("Queue ordered from tail\n");
   temp = tail;
   while (temp != NULL) {
      printf("%d\n", temp->number);
      temp = temp->next;
   }
   printf("\n");
}

void freeQueue(datatype *tail) {
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

	/* create a queue of five records */
   printf("Add 7 to queue\n");
   tail = addElement(7, tail);
   printQueue(tail);

   printf("Add 3 to queue\n");
   tail = addElement(3, tail);
   printQueue(tail);

   printf("Add 5 to queue\n");
   tail = addElement(5, tail);
   printQueue(tail);

   printf("Remove element from queue\n");
   tail = removeElement(tail);
   printQueue(tail);

   printf("Remove element from queue\n");
   tail = removeElement(tail);
   printQueue(tail);

   printf("Add 10 to queue\n");
   tail = addElement(10, tail);
   printQueue(tail);

   printf("Add 3 to queue\n");
   tail = addElement(3, tail);
   printQueue(tail);

   freeQueue(tail);

   return(1);
}
