/*
 * ------------------------------------------------------------------
 *	OrderedList.h - Ordered list ADT prototypes/data declarations
 *
 *	Author: Andrew Crowell
 * ------------------------------------------------------------------
 */

#ifndef ORDERED_LIST_H
#define ORDERED_LIST_H

#include <stdlib.h>
#include "List.h"

typedef int (*OrderedComparator)(void *, void*);
/* A set of ordered objects stored in a 2-way chained linear sequence */
typedef struct OrderedList_t
{
	List* list;
	OrderedComparator comparator;
} OrderedList;

OrderedList* OrderedListCreate(OrderedComparator comparator);
int OrderedListDestroy(OrderedList* this);
int OrderedListClear(OrderedList* this);
OrderedList* OrderedListHead(OrderedList* this);
OrderedList* OrderedListTail(OrderedList* this);
OrderedList* OrderedListNext(OrderedList* this);
OrderedList* OrderedListPrevious(OrderedList* this);
OrderedList* OrderedListSetPosition(OrderedList* this, int index);
OrderedList* OrderedListInsert(OrderedList* this, void* item);
void* OrderedListSearch(OrderedList* this, void* item);
void* OrderedListGetCurrent(OrderedList* this);
void* OrderedListDeleteCurrent(OrderedList* this);
int OrderedListIsEmpty(OrderedList* this);
int OrderedListGetLength(OrderedList* this);
int OrderedListGetPosition(OrderedList* this);

#endif
