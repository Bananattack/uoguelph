/*
	CIS*2750
	Andrew G. Crowell
	0545826
	acrowell@uoguelph.ca
*/

#ifndef PLIST_H
#define PLIST_H

#include <stdlib.h>
#include <string.h>
#include "adt/List.h"

/* ParameterList ADT */
typedef struct ParameterList_t
{
	List* list;
	int done;
} ParameterList;

ParameterList* PL_create(List* q);
void PL_destroy(ParameterList* this);
void PL_reset(ParameterList* this);
char* PL_next(ParameterList* this);
int PL_getLength(ParameterList* this);

#endif
