#ifndef PLIST_H
#define PLIST_H

#include <stdlib.h>
#include <string.h>
#include "adt/Queue.h"

/* ParameterList ADT */
typedef struct ParameterList_t
{
	Queue* queue;
} ParameterList;

ParameterList* PL_create(Queue* q);
void PL_destroy(ParameterList* this);
char* PL_next(ParameterList* this);

#endif
