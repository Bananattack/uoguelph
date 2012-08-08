/*
 * ------------------------------------------------------------------
 *	OrderedList.c - Ordered list ADT implementation
 *
 *	Author: Andrew Crowell
 * ------------------------------------------------------------------
 */

#include "OrderedList.h"

/*
 * ---------------------------------------------------------
 *	OrderedListCreate
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
OrderedList* OrderedListCreate(OrderedComparator comparator)
{

	OrderedList* this = calloc(1, sizeof(OrderedList));

	if (!this)
	{
	    return NULL;
	}

	this->list = listCreate();
	this->comparator = comparator;

	return this;
}

/*
 * ---------------------------------------------------------
 *	OrderedListDestroy
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
int OrderedListDestroy(OrderedList* this)
{
	int result = listDelete(this->list);
	free(this);
	return result;
}

/*
 * ---------------------------------------------------------
 *	OrderedListClear
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
int OrderedListClear(OrderedList* this)
{
	return listClear(this->list);
}

/*
 * ---------------------------------------------------------
 *	OrderedListHead
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
OrderedList* OrderedListHead(OrderedList* this)
{
	return (listHead(this->list) == NULL) ? NULL : this;
}

/*
 * ---------------------------------------------------------
 *	OrderedListTail
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
OrderedList* OrderedListTail(OrderedList* this)
{
	return (listTail(this->list) == NULL) ? NULL : this;
}
/*
 * ---------------------------------------------------------
 *	OrderedListNext
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
OrderedList* OrderedListNext(OrderedList* this)
{
	return (listNext(this->list) == NULL) ? NULL : this;
}

/*
 * ---------------------------------------------------------
 *	OrderedListPrevious
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
OrderedList* OrderedListPrevious(OrderedList* this)
{
	return (listPrev(this->list) == NULL) ? NULL : this;
}

/*
 * ---------------------------------------------------------
 *	OrderedListSetPosition
 *		- Move to the "Nth" element in the provide dlist
 *		- NOTE: elements are counted from 1 (not 0)
 * ---------------------------------------------------------
 *	PRE:
 *		0 < index <= element_count
 * ---------------------------------------------------------
 *	POST:
 *		current & position now reference the "Nth" element
 *		in this list; returns a reference to this
 * ---------------------------------------------------------
 *	ERRORS:
 *		NULL : element index out of bounds (index)
 * ---------------------------------------------------------
 */
OrderedList* OrderedListSetPosition(OrderedList* this, int index)
{
	return (listMoveToNth(this->list, index) == NULL) ? NULL : this;
}

/*
 * ---------------------------------------------------------
 *	OrderedListInsert
 *		- Insert a new element containing the provided
 *		  data reference according to ordering followed by list 
 *		- NOTE: current reference remains unchanged
 * ---------------------------------------------------------
 *	PRE:
 *		void
 * ---------------------------------------------------------
 *	POST:
 *		element containing data reference is inserted into
 *		this list, or is installed as
 *		the new head of the list if it was emtpy; returns
 *		a reference to this
 * ---------------------------------------------------------
 *	ERRORS:
 *		NULL : new_element() failure
 * ---------------------------------------------------------
 */
OrderedList* OrderedListInsert(OrderedList* this, void* item)
{
	OrderedList* iterator;
	int pos = OrderedListGetPosition(this);
	void* current;
	OrderedList* result = NULL;

	iterator = OrderedListHead(this);
	/* Empty list, insert a new element */
	if (iterator == NULL)
	{
		return (listAddAfter(this->list, item) == NULL) ? NULL : this;		
	}
	current = OrderedListGetCurrent(this);

	/* Start of the list is smallest */
	if (this->comparator(item, current) < 0)
	{
		return (listAddBefore(this->list, item) == NULL) ? NULL : this;
	}

	iterator = OrderedListNext(this);
	current = OrderedListGetCurrent(this);
	while (iterator != NULL && this->comparator(item, current) >= 0)
	{
		iterator = OrderedListNext(this);
		current = OrderedListGetCurrent(this);
	}

	/* Add at tail*/
	if(iterator == NULL)
	{
		result = (listAddAfter(this->list, item) == NULL) ? NULL : this;
	}
	/* Add in between two items */
	else
	{
		result = (listAddBefore(this->list, item) == NULL) ? NULL : this;
	}
	OrderedListSetPosition(this, pos);

	return result;
}

/*
 * ---------------------------------------------------------
 *	OrderedListSearch
 *		- Sequentially searches for an
 *		  entry in the list.
 * ---------------------------------------------------------
 *	PRE:
 *		list is non-empty
 * ---------------------------------------------------------
 *	POST:
 *		returns a reference to the item if found
 *		or NULL if not found.
 * ---------------------------------------------------------
 *	ERRORS:
 *		NULL : list is empty
 *		NULL : list does not contain supplied item
 * ---------------------------------------------------------
 */
void* OrderedListSearch(OrderedList* this, void* item)
{
	OrderedList* iterator;
	void* current;

	iterator = OrderedListHead(this);
	current = OrderedListGetCurrent(this);
	while (iterator != NULL)
	{
		if(this->comparator(item, current) == 0)
		{
			return current;
		}
		iterator = OrderedListNext(this);
		current = OrderedListGetCurrent(this);
	}
	return NULL;
}

/*
 * ---------------------------------------------------------
 *	OrderedListGetCurrent
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
void* OrderedListGetCurrent(OrderedList* this)
{
	return listGetCurrent(this->list);
}

/*
 * ---------------------------------------------------------
 *	OrderedListDeleteCurrent
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
void* OrderedListDeleteCurrent(OrderedList* this)
{
	return listDelCurrent(this->list);
}

/*
 * ---------------------------------------------------------
 *	OrderedListIsEmpty
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
int OrderedListIsEmpty(OrderedList* this)
{
	return listIsEmpty(this->list);
}

/*
 * ---------------------------------------------------------
 *	OrderedListGetLength
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
int OrderedListGetLength(OrderedList* this)
{
	return listLength(this->list);
}

/*
 * ---------------------------------------------------------
 *	OrderedListGetPosition
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
int OrderedListGetPosition(OrderedList* this)
{
	return listPosition(this->list);
}
