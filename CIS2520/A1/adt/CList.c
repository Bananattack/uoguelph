/*
	CList.c - Contiguous List ADT implementation

	Author: Andrew Crowell
*/

#include "CList.h"

/*
	Resizes the list if the capacity cannot hold one more element.
	Used internally, do not call this otherwise.
*/
bool __listResize(List* this)
{
	/* If the current capacity can't contain one more element, double the capacity.*/
	if(this->element_count + 1 >= this->capacity)
	{
		this->capacity *= 2;
		this->element = realloc(this->element, sizeof(void*) * this->capacity);
		if(!this->element)
		{
			return false;
		}
	}
	return true;
}

/*
	listCreate
		- Create new list initialized to empty

	PRE:
		void

	POST:
		new list is allocated and initialized; returns a
		reference to new list

	ERRORS:
		NULL : memory allocation failure (allocation)
*/
List* listCreate(void)
{
	List* this = malloc(sizeof(List));
	if (!this)
	{
	    return NULL;
	}

	this->capacity = 1;
	this->element = malloc(sizeof(void*) * this->capacity);
	this->element_count = 0;
	this->position = 0;

	return this;

}

/*
	listDelete
		- De-allocate all memory associated with provided
		  list object

	PRE:
		void

	POST:
		this list is destroyed; returns number of elements
		that were in the list

	ERRORS:
		void

*/
int listDelete(List* this)
{
	int result = 0;

	if (this)
	{
		result = listClear(this);
		free(this->element);
		free(this);
	}

	return result;
}

/*

	listClear
		- De-allocate all memory associated with element
		  structures of provided list, resetting state of
		  list to empty

	PRE:
		void

	POST:
		this list is empty; returns number of elements that
		were in the list

	ERRORS:
		void

*/
int listClear(List* this)
{
	int i;
	int result;

	/* Deallocate element memory*/
	for (i = 0; i < this->element_count; i++)
	{
		free(this->element[i]);
	}

	/* Save number of elements in list, then reset to empty*/
	result = this->element_count;
	this->element_count = 0;
	this->position = 0;

	return result;
}

/*

	listHead
		- Adjust position to the beginning of provided list
		- NOTE: this operation is non-destructive on an
		  empty list

	PRE:
		list is non-empty

	POST:
		current & position reference the first element in
		this list; returns a reference to this

	ERRORS:
		NULL : list is empty

*/
List* listHead(List* this)
{
	if (this->element_count)
	{
		this->position = 1;
		return this;
	}
	else
	{
		return NULL;
	}
}

/*

	listTail
		- Adjust position to the end of provided list
		- NOTE: this operation is non-destructive on an
		  empty list

	PRE:
		list is non-empty

	POST:
		current & position reference the last element in
		this list; returns a reference to this

	ERRORS:
		NULL : list is empty

*/
List* listTail(List* this)
{
	if (this->element_count)
	{
		this->position = this->element_count;
		return this;
	}	
	else
	{
		return NULL;
	}
}

/*

	listPrev
		- Move the the previous element in the provided list

	PRE:
		list is non-empty; list is not positioned at the
		first element

	POST:
		current & postition now reference the element
		preceeding the current one in this list; returns a
		reference to this

	ERRORS:
		NULL : list is empty
		NULL : already at beginning of list

*/
List* listPrev(List* this)
{
	if (!this->element_count || this->position <= 1)
	{
		return NULL;
	}

	this->position--;

	return this;
}

/*

	listNext
		- Move the the next element in the provided list

	PRE:
		list is non-empty; list is not positioned at the
		last element

	POST:
		current & position now reference the element
		succeeding the current one in this list;  returns 
		a reference to this

	ERRORS:
		NULL : list is empty
		NULL : already at end of list

*/
List* listNext(List* this)
{
	if (!this->element_count || this->position >= this->element_count)
	{
		return NULL;
	}

	this->position++;

	return this;
}

/*

	listMoveToNth
		- Move to the "Nth" element in the provide dlist
		- NOTE: elements are counted from 1 (not 0)

	PRE:
		0 < n <= element_count

	POST:
		current & position now reference the "Nth" element
		in this list; returns a reference to this

	ERRORS:
		NULL : element index out of bounds (index)

*/
List* listMoveToNth(List* this, int n)
{
	if (this->position < 1 || this->position > this->element_count)
	{
		return NULL;
	}

	this->position = n;

	return this;
}

/*

	listAddAfter
		- Insert a new element containing the provided
		  data reference in list AFTER current element 
		- NOTE: current reference remains unchanged

	PRE:
		void

	POST:
		element containing data reference is inserted into
		this list AFTER the current one, or is installed as
		the new head of the list if it was emtpy; returns
		a reference to this

	ERRORS:
		NULL : new_element() failure

*/
List* listAddAfter(List* this, void* data)
{	
	int i;

	if (!__listResize(this))
	{
		return NULL;
	}

	/* Empty lists need to shift their start position to the head once an element is added.*/
	if (!this->position)
	{
		this->position = 1;
	}
	if(this->element_count)
	{
		/* Shove everything over so we can fit the new element after position*/  
		for (i = this->element_count - 1; i >= this->position; i--)
		{
			this->element[i + 1] = this->element[i];
		}
		this->element[this->position] = data;
	}
	else
	{
		this->element[0] = data;
	}


	this->element_count++;

	return this;
}

/*

	listAddBefore
		- Insert a new element containing the provided
		  data reference in list BEFORE current element 
		- NOTE: current reference remains unchanged

	PRE:
		void

	POST:
		element containing data reference is inserted into
		this list BEFORE the current one, or is installed as
		the new head of the list if it was emtpy; returns
		a reference to this

	ERRORS:
		NULL : new_element() failure

*/
List* listAddBefore(List* this, void* data)
{
	int i;

	if (!__listResize(this))
	{
		return NULL;
	}

	/* Empty lists need to shift their start position to the head once an element is added.*/
	if (!this->position)
	{
		this->position = 1;
	}

	if(this->element_count)
	{
		/* Shove everything over so we can fit the new element at the position*/  
		for (i = this->element_count - 1; i >= this->position - 1; i--)
		{
			this->element[i + 1] = this->element[i];
		}
		this->element[this->position - 1] = data;
	}
	else
	{
		this->element[0] = data;
	}

	this->element_count++;

	return this;
}

/*

	listGetCurrent
		- Obtain a reference to data from the current
		  element data in the provided list

	PRE:
		list is non-empty

	POST:
		returns a reference to the data in the current
		element in this list

	ERRORS:
		NULL : list is empty

*/
void* listGetCurrent(List* this)
{
	if (!this->element_count)
	{
		return NULL;
	}
	return this->element[this->position - 1];
}

/*

	listSetCurrent
		- Reassign a data reference within the current
		  element of the provided list

	PRE:
		list is non-empty

	POST:
		returns a reference to the previous (overwritten)
		data in the current element of this list

	ERRORS:
		NULL : list is empty

*/
void* listSetCurrent(List* this, void* value)
{
	void* old_value;

	if (!this->element_count)
	{
		return NULL;
	}

	old_value = this->element[this->position - 1];
	this->element[this->position - 1] = value; 

	return old_value;
}

/*

	listDelCurrent
		- Remove current element from the provided list and
		  obtain a reference to the data in the element

	PRE:
		list is non-empty

	POST:
		returns a reference to the data in the current
		element in this list which no longer occurs in
		the list;  new current element is removed's
		successor

	ERRORS:
		NULL : list is empty

*/
void* listDelCurrent(List* this)
{
	int i;
	void* old_value;
	
	if (!this->element_count)
	{
		return NULL;
	}

	old_value = this->element[this->position - 1];

	/* Single item list*/
	if (this->element_count == 1)
	{
		this->element_count = 0;
		this->position = 0;
	}
	/* Multi-item list*/
	else
	{
		/* Shove everything over one, effectively deleting an element*/
		for (i = this->position - 1; i < this->element_count - 1; i++)
		{
			this->element[i] = this->element[i + 1];
		}
		this->element_count--;
		this->position--;
	}

	return old_value;
}

/*

	listIsEmpty
		- Determine if provided list is empty

	PRE:
		void

	POST:
		returns 1 if this is empty, 0 otherwise

	ERRORS:
		void

*/
int listIsEmpty(List* this)
{
	return this->element_count == 0;
}

/*

	listLength
		- Determine number of elements (length) in
		  provided list

	PRE:
		void

	POST:
		returns value of element_count

	ERRORS:
		void

*/
int listLength(List* this)
{
	return this->element_count;
}

/*

	listPosition
		- Obtain numeric value of current position in
		  provided list

	PRE:
		void

	POST:
		returns value of position

	ERRORS:
		void

*/
int listPosition(List* this)
{
	return this->position;
}
