/*
	CIS*2750
	Andrew G. Crowell
	0545826
	acrowell@uoguelph.ca
*/
#include "ParameterList.h"

/*
	Creates a new parameter list object
	PRE: queue is a valid queue pointer.
	POST:	Returns a new parameter list object that acts on the
		the queue of strings on success,
		NULL otherwise (memory allocation failure)
*/
ParameterList* PL_create(List* l)
{
	ParameterList* this = malloc(sizeof(ParameterList));
	this->list = l;
	this->done = 0;
	listHead(this->list);
	return this;
}

/*
	Destroys a parameter list object
	PRE: n/a
	POST:	all memory associated with the parameter list is freed
*/
void PL_destroy(ParameterList* this)
{
	listDelete(this->list);
	free(this);
}

/*
	Resets a parameter list, relocating iteration to the beginning of the list again.
	PRE: n/a
	POST:	position is reset to the start
*/
void PL_reset(ParameterList* this)
{
	this->done = 0;
	listHead(this->list);
}

/*
	Obtain the next item in a parameter list
	PRE: n/a
	POST:	Returns the next item in the parameter list,
		NULL if there are no items remaining in the list
*/
char* PL_next(ParameterList* this)
{
	char* value = listGetCurrent(this->list);
	if (this->done)
	{
		return NULL;
	}
	if(listNext(this->list) == NULL)
	{
		this->done = 1;
	}
	return value;
}

/*
	Resets a parameter list, relocating iteration to the beginning of the list again.
	PRE: n/a
	POST:	position is reset to the start
*/
int PL_getLength(ParameterList* this)
{
	return listLength(this->list);
}
