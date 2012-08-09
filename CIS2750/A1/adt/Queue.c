#include "Queue.h"

/*
	queueDepart
		- Removes an item from the front of the queue and returns its value.

	PRE:
		queue is non-empty

	POST:
		returns a reference to the data removed from the first in the queue.

	ERRORS:
		NULL: stack is empty
*/
void* queueDepart(Queue* this)
{
	listHead(this);
	return listDelCurrent(this);
}

/*
	queueArrive
		Adds an item to the end of the queue

	PRE:
		void

	POST:
		a new data reference is inserted at the last in the queue

	ERRORS:
		void
*/
void queueArrive(Queue* this, void* data)
{
	listTail(this);
	listAddAfter(this, data);
}
