/*
 * ------------------------------------------------------------------
 *	List.h - List ADT prototypes/data declarations
 *
 *	Author: David McCaughan (1995, CIS*2520 revision 2006)
 * ------------------------------------------------------------------
 */

#ifndef LIST_H
#define LIST_H


#include <stdio.h>
#include "Element.h"


/*
 * --- TYPE: List ----------------------------------------------------------
 *	A set of objects stored in a 2-way chained linear sequence
 * -------------------------------------------------------------------------
 */
typedef struct List_t
{
	int     element_count;		/* number of elements in list */
	int     position;			/* current (numeric) position in list */
	Element *elements;			/* list of elements */
	Element *current;			/* current element */
} List;


/* --- file: List.c --- */

List * listCreate     (void);
int    listDelete     (List *);
int    listClear      (List *);
List * listHead       (List *);
List * listTail       (List *);
List * listPrev       (List *);
List * listNext       (List *);
List * listMoveToNth  (List *, int);
List * listAddAfter   (List *, void *);
List * listAddBefore  (List *, void *);
void * listGetCurrent (List *);
void * listSetCurrent (List *, void *);
void * listDelCurrent (List *);
int    listIsEmpty    (List *);
int    listLength     (List *);
int    listPosition   (List *);


#endif
