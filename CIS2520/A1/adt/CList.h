/*
	List.h - Contiguous List ADT prototypes/data declarations

	Author: Andrew Crowell
 */

#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


/* List - A set of objects stored in a 2-way chained linear sequence */
typedef struct List_t
{
	/* Current capacity of the list */
	int capacity;
	/* Number of elements currently in list. */
	int element_count;
	/* Current position in the list. An index in the element array + 1. */
	int position;
	/* The dynamic array of all the element data, using contiguous memory */
	void** element;
} List;

/* Private internal operations. */
bool __listResize(List* this);

List* listCreate();
int listDelete(List* this);
int listClear(List* this);
List* listHead(List* this);
List* listTail(List* this);
List* listPrev(List* this);
List* listNext(List* this);
List* listMoveToNth(List* this, int);
List* listAddAfter(List* this, void* data);
List* listAddBefore(List* this, void* data);
void* listGetCurrent(List* this);
void* listSetCurrent(List* this, void* value);
void* listDelCurrent(List* this);
int listIsEmpty(List* this);
int listLength(List* this);
int listPosition(List* this);


#endif
