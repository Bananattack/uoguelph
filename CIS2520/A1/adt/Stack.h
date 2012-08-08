/*
	Stack.c - Last-In/First-Out Sequence ADT prototypes/data declarations

	Author: Andrew Crowell
*/

#ifndef STACK_H
#define STACK_H

#include "CList.h"

/* Stacks pretty much wrap lists and add extra insertion/deletion functionality, so save us some overhead. */
typedef List Stack;

/* Inline the stack functions where possible. */
#define stackCreate(this) (listCreate(this))
#define stackDelete(this) (listDelete(this))
#define stackClear(this) (listClear(this))
#define stackIsEmpty(this) (listIsEmpty(this))
#define stackLength(this) (listLength(this))

void* stackPop(Stack* this);
void stackPush(Stack* this, void* data);


#endif
