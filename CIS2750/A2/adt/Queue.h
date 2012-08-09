/*
	Queue.c - First-In/First-Out Sequence ADT prototypes/data declarations

	Author: Andrew Crowell
*/

#ifndef QUEUE_H
#define QUEUE_H

#include "List.h"

/* Queues pretty much wrap lists and add extra insertion/deletion functionality, so save us some overhead. */
typedef List Queue;

/* Inline the queue functions where possible. */
#define queueCreate(this) (listCreate(this))
#define queueDelete(this) (listDelete(this))
#define queueClear(this) (listClear(this))
#define queueIsEmpty(this) (listIsEmpty(this))
#define queueLength(this) (listLength(this))

void* queueDepart(Queue* this);
void queueArrive(Queue* this, void* data);


#endif
