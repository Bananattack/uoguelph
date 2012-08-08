/*
	Stack.c - Last-In/First-Out Sequence ADT

	Author: Andrew Crowell
*/
#include "Stack.h"

/*
	stackPop
		- Removes an item from the stack and returns its value.

	PRE:
		stack is non-empty

	POST:
		returns a reference to the data removed from the top of the stack

	ERRORS:
		NULL: stack is empty
*/
void* stackPop(Stack* this)
{
	listHead(this);
	return listDelCurrent(this);
}

/*
	stackPush
		Adds an item to the top of the stack

	PRE:
		void

	POST:
		a new data reference is inserted into this stack at the top

	ERRORS:
		void
*/
void stackPush(Stack* this, void* data)
{
	listHead(this);
	listAddBefore(this, data);
}
