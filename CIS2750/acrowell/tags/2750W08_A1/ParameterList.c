#include "ParameterList.h"

/*
	Creates a new parameter list object
	PRE: queue is a valid queue pointer.
	POST:	Returns a new parameter list object that acts on the
		the queue of strings on success,
		NULL otherwise (memory allocation failure)
*/
ParameterList* PL_create(Queue* q)
{
	ParameterList* this = malloc(sizeof(ParameterList));
	this->queue = q;
	return this;
}

/*
	Destroys a parameter list object
	PRE: n/a
	POST:	all memory associated with the parameter list is freed
*/
void PL_destroy(ParameterList* this)
{
	queueDelete(this->queue);
	free(this);
}

/*
	Obtain the next item in a parameter list
	PRE: n/a
	POST:	Returns the next item in the parameter list,
		NULL if there are no items remaining in the list
*/
char* PL_next(ParameterList* this)
{
	return queueDepart(this->queue);
}
