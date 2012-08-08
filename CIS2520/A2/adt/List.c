/*
 * ------------------------------------------------------------------
 *	List.c - List ADT implementation
 *
 *	Author: David McCaughan (1995, CIS*2520 revision 2006)
 * ------------------------------------------------------------------
 */

#include <stdlib.h>
#include <stdio.h>
#include "List.h"
#include "Element.h"


/*
 * ---------------------------------------------------------
 *	listCreate
 *		- Create new list initialized to empty
 * ---------------------------------------------------------
 *	PRE:
 *		void
 * ---------------------------------------------------------
 *	POST:
 *		new list is allocated and initialized; returns a
 *		reference to new list
 * ---------------------------------------------------------
 *	ERRORS:
 *		NULL : memory allocation failure (allocation)
 * ---------------------------------------------------------
 */
List *listCreate(void)
{

	List *new = NULL;		/* new list */

	if (!(new = calloc(1,sizeof(List))))
	    return(NULL);

	new->element_count = 0;
	new->position = 0;
	new->elements = NULL;
	new->current = NULL;

	return(new);

} /* listCreate */



/*
 * ---------------------------------------------------------
 *	listDelete
 *		- De-allocate all memory associated with provided
 *		  list object
 * ---------------------------------------------------------
 *	PRE:
 *		void
 * ---------------------------------------------------------
 *	POST:
 *		this list is destroyed; returns number of elements
 *		that were in the list
 * ---------------------------------------------------------
 *	ERRORS:
 *		void
 * ---------------------------------------------------------
 */
int listDelete(List *this)
{
	int retval = 0;		/* return value */

	if (this)
	{
		retval = listClear(this);
		free(this);
	}

	return(retval);

} /* listDelete */



/*
 * ---------------------------------------------------------
 *	listClear
 *		- De-allocate all memory associated with element
 *		  structures of provided list, resetting state of
 *		  list to empty
 * ---------------------------------------------------------
 *	PRE:
 *		void
 * ---------------------------------------------------------
 *	POST:
 *		this list is empty; returns number of elements that
 *		were in the list
 * ---------------------------------------------------------
 *	ERRORS:
 *		void
 * ---------------------------------------------------------
 */
int listClear(List *this)
{
	int i;			/* loop index */
	int retval;		/* return value */
	Element *cur;	/* current element - traversal */

	/*
	 * de-allocate element memory
	 */
	if (this->elements)
	{
		this->current = this->elements;
		for (i = 0; i < this->element_count; i++)
		{
			cur = this->current;
			this->current = elementGetRight(this->current);
			elementDelete(cur);
		}
	}

	/*
	 * save number of elements in list then reset to empty
	 */
	retval = this->element_count;
	this->element_count = 0;
	this->position = 0;
	this->elements = NULL;
	this->current = NULL;

	return(retval);

} /* listClear */



/*
 * ---------------------------------------------------------
 *	listHead
 *		- Adjust position to the beginning of provided list
 *		- NOTE: this operation is non-destructive on an
 *		  empty list
 * ---------------------------------------------------------
 *	PRE:
 *		list is non-empty
 * ---------------------------------------------------------
 *	POST:
 *		current & position reference the first element in
 *		this list; returns a reference to this
 * ---------------------------------------------------------
 *	ERRORS:
 *		NULL : list is empty
 * ---------------------------------------------------------
 */
List *listHead(List *this)
{
	if (this->elements != NULL)
	{
		this->current = this->elements;
		this->position = 1;
		return(this);
	}
	else
		return(NULL);

} /* listHead */



/*
 * ---------------------------------------------------------
 *	listTail
 *		- Adjust position to the end of provided list
 *		- NOTE: this operation is non-destructive on an
 *		  empty list
 * ---------------------------------------------------------
 *	PRE:
 *		list is non-empty
 * ---------------------------------------------------------
 *	POST:
 *		current & position reference the last element in
 *		this list; returns a reference to this
 * ---------------------------------------------------------
 *	ERRORS:
 *		NULL : list is empty
 * ---------------------------------------------------------
 */
List *listTail(List *this)
{
	if (this->elements != NULL)
	{
		this->current = elementGetLeft(this->elements);
		this->position = this->element_count;
		return(this);
	}	
	else
		return(NULL);

} /* listTail */



/*
 * ---------------------------------------------------------
 *	listPrev
 *		- Move the the previous element in the provided list
 * ---------------------------------------------------------
 *	PRE:
 *		list is non-empty; list is not positioned at the
 *		first element
 * ---------------------------------------------------------
 *	POST:
 *		current & postition now reference the element
 *		preceeding the current one in this list; returns a
 *		reference to this
 * ---------------------------------------------------------
 *	ERRORS:
 *		NULL : list is empty
 *		NULL : already at beginning of list
 * ---------------------------------------------------------
 */
List *listPrev(List *this)
{
	/*
	 * note that this explicity covers "at beginning of list", and
	 * implicitly covers "empty list", and "single element list" cases.
	 */
	if (this->current == this->elements)
		return(NULL);

	this->current = elementGetLeft(this->current);
	this->position--;

	return(this);

} /* listPrev */



/*
 * ---------------------------------------------------------
 *	listNext
 *		- Move the the next element in the provided list
 * ---------------------------------------------------------
 *	PRE:
 *		list is non-empty; list is not positioned at the
 *		last element
 * ---------------------------------------------------------
 *	POST:
 *		current & position now reference the element
 *		succeeding the current one in this list;  returns 
 *		a reference to this
 * ---------------------------------------------------------
 *	ERRORS:
 *		NULL : list is empty
 *		NULL : already at end of list
 * ---------------------------------------------------------
 */
List *listNext(List *this)
{
	/*
	 * must explicitly cover "at end of list" as well as "empty list"
	 * and "single element list" cases
	 */
	if ((this->position >= this->element_count)
			|| (!this->elements) || (this->element_count < 2))
		return(NULL);

	this->current = elementGetRight(this->current);
	this->position++;

	return(this);

} /* listNext */



/*
 * ---------------------------------------------------------
 *	listMoveToNth
 *		- Move to the "Nth" element in the provide dlist
 *		- NOTE: elements are counted from 1 (not 0)
 * ---------------------------------------------------------
 *	PRE:
 *		0 < n <= element_count
 * ---------------------------------------------------------
 *	POST:
 *		current & position now reference the "Nth" element
 *		in this list; returns a reference to this
 * ---------------------------------------------------------
 *	ERRORS:
 *		NULL : element index out of bounds (index)
 * ---------------------------------------------------------
 */
List *listMoveToNth(List *this, int n)
{
	int i;			/* loop index */
	int offset;		/* offset to new location from current */
	int ofor;		/* forward and ...                           */
	int obak;		/*         ... backward offsets respectively */

	/*
	 * this should avoid ANY illegal situation (i.e. an "nth" element
	 * exists) - excepting destructive user interference
	 */
	if ((n < 1) || (n > this->element_count))
		return(NULL);

	/*
	 * determine offset & direction of pointer movement from current:
	 * (adds a couple of arithmetic operations over linear search
	 * however may save time if movement is highly localized or a
	 * regular progression through the list (i.e. by 2s))
	 */

	obak = (this->position - n + this->element_count) % this->element_count;
	ofor = (n - this->position + this->element_count) % this->element_count;
	offset = (obak < ofor) ? obak : ofor;
	if (ofor < obak)
		/* move forward */
		for (i = 0; i < offset; i++)
			this->current = elementGetRight(this->current);
	else
		/* move backward */
		for (i = 0; i < offset; i++)
			this->current = elementGetLeft(this->current);

	this->position = n;

	return(this);

} /* listMoveToNth */



/*
 * ---------------------------------------------------------
 *	listAddAfter
 *		- Insert a new element containing the provided
 *		  data reference in list AFTER current element 
 *		- NOTE: current reference remains unchanged
 * ---------------------------------------------------------
 *	PRE:
 *		void
 * ---------------------------------------------------------
 *	POST:
 *		element containing data reference is inserted into
 *		this list AFTER the current one, or is installed as
 *		the new head of the list if it was emtpy; returns
 *		a reference to this
 * ---------------------------------------------------------
 *	ERRORS:
 *		NULL : new_element() failure
 * ---------------------------------------------------------
 */
List *listAddAfter(List *this, void *data)
{
	Element *new;		/* new element */

	if (!(new = elementCreate(data)))
		return(NULL);

	/*
	 * link into list
	 */
	if (!this->elements)
	{
		this->elements = new;
		this->current = this->elements;
		elementSetLeft(this->current,new);
		elementSetRight(this->current,new);
		this->position++;
	}
	else
	{
		elementSetLeft(elementGetRight(this->current),new);
		elementSetRight(new,elementGetRight(this->current));
		elementSetLeft(new,this->current);
		elementSetRight(this->current,new);
	}
	this->element_count++;

	return(this);

} /* listAddAfter */




/*
 * ---------------------------------------------------------
 *	listAddBefore
 *		- Insert a new element containing the provided
 *		  data reference in list BEFORE current element 
 *		- NOTE: current reference remains unchanged
 * ---------------------------------------------------------
 *	PRE:
 *		void
 * ---------------------------------------------------------
 *	POST:
 *		element containing data reference is inserted into
 *		this list BEFORE the current one, or is installed as
 *		the new head of the list if it was emtpy; returns
 *		a reference to this
 * ---------------------------------------------------------
 *	ERRORS:
 *		NULL : new_element() failure
 * ---------------------------------------------------------
 */
List *listAddBefore(List *this, void *data)
{
	Element *new;	/* new element structure */

	if (!(new = elementCreate(data)))
		return(NULL);

	/*
	 * link into list
	 */
	if (!this->elements)
	{
		this->elements = new;
		this->current = this->elements;
		elementSetLeft(this->current,new);
		elementSetRight(this->current,new);
	}
	else
	{
		elementSetRight(elementGetLeft(this->current),new);
		elementSetLeft(new,elementGetLeft(this->current));
		elementSetRight(new,this->current);
		elementSetLeft(this->current,new);
		/*
		 * must adjust head of list if addition is at front of list
		 */
		if (this->current == this->elements)
			this->elements = new;
	}
	this->element_count++;
	this->position++;

	return(this);

} /* listAddBefore */



/*
 * ---------------------------------------------------------
 *	listGetCurrent
 *		- Obtain a reference to data from the current
 *		  element data in the provided list
 * ---------------------------------------------------------
 *	PRE:
 *		list is non-empty
 * ---------------------------------------------------------
 *	POST:
 *		returns a reference to the data in the current
 *		element in this list
 * ---------------------------------------------------------
 *	ERRORS:
 *		NULL : list is empty
 * ---------------------------------------------------------
 */
void *listGetCurrent(List *this)
{
	if (!this->elements)
		return(NULL);

	return(elementGetData(this->current));	

} /* listGetCurrent */



/*
 * ---------------------------------------------------------
 *	listSetCurrent
 *		- Reassign a data reference within the current
 *		  element of the provided list
 * ---------------------------------------------------------
 *	PRE:
 *		list is non-empty
 * ---------------------------------------------------------
 *	POST:
 *		returns a reference to the previous (overwritten)
 *		data in the current element of this list
 * ---------------------------------------------------------
 *	ERRORS:
 *		NULL : list is empty
 * ---------------------------------------------------------
 */
void *listSetCurrent(List *this, void *data)
{
	void *dval;			/* reference to data value */

	if (!this->elements)
		return(NULL);

	dval = elementGetData(this->current);
	elementSetData(this->current,data);

	return(dval);	

} /* listSetCurrent */



/*
 * ---------------------------------------------------------
 *	listDelCurrent
 *		- Remove current element from the provided list and
 *		  obtain a reference to the data in the element
 * ---------------------------------------------------------
 *	PRE:
 *		list is non-empty
 * ---------------------------------------------------------
 *	POST:
 *		returns a reference to the data in the current
 *		element in this list which no longer occurs in
 *		the list;  new current element is removed's
 *		successor
 * ---------------------------------------------------------
 *	ERRORS:
 *		NULL : list is empty
 * ---------------------------------------------------------
 */
void *listDelCurrent(List *this)
{
	Element *el;		/* reference to item to be deleted */
	void *dval;			/* reference to data value */

	if (!this->elements)
		return(0);

	/*
	 * note element for deletion
	 */
	el = this->current;

	/*
	 * re-link list to remove current node
	 */
	elementSetRight(elementGetLeft(this->current),
			elementGetRight(this->current));
	elementSetLeft(elementGetRight(this->current),
			elementGetLeft(this->current));
	this->current = elementGetRight(this->current);

	/*
	 * SPECIAL CASES:
	 */
	/* single element list - deleting only item */
	if (this->element_count == 1)
	{
		this->elements = this->current = NULL;
		this->position = 0;
	}
	/* end node removed - wrapping current to first */
	else if (this->elements == this->current)
		this->position = 1;
	/* first node removed - updating head of list */
	else if (this->elements == el)
		this->elements = this->current;

	this->element_count--;

	dval = elementGetData(el);
	elementDelete(el);

	return(dval);

} /* listDelCurrent */



/*
 * ---------------------------------------------------------
 *	listIsEmpty
 *		- Determine if provided list is empty
 * ---------------------------------------------------------
 *	PRE:
 *		void
 * ---------------------------------------------------------
 *	POST:
 *		returns 1 if this is empty, 0 otherwise
 * ---------------------------------------------------------
 *	ERRORS:
 *		void
 * ---------------------------------------------------------
 */
int listIsEmpty(List *this)
{
	return(this->element_count == 0);

} /* listIsEmpty */



/*
 * ---------------------------------------------------------
 *	listLength
 *		- Determine number of elements (length) in
 *		  provided list
 * ---------------------------------------------------------
 *	PRE:
 *		void
 * ---------------------------------------------------------
 *	POST:
 *		returns value of element_count
 * ---------------------------------------------------------
 *	ERRORS:
 *		void
 * ---------------------------------------------------------
 */
int listLength(List *this)
{
	return(this->element_count);

} /* listLength */



/*
 * ---------------------------------------------------------
 *	listPosition
 *		- Obtain numeric value of current position in
 *		  provided list
 * ---------------------------------------------------------
 *	PRE:
 *		void
 * ---------------------------------------------------------
 *	POST:
 *		returns value of position
 * ---------------------------------------------------------
 *	ERRORS:
 *		void
 * ---------------------------------------------------------
 */
int listPosition(List *this)
{
	return(this->position);

} /* listPosition */
