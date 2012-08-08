/*
 * ------------------------------------------------------------------
 *	Element.c - Element ADT implementation
 *
 *	Author: David McCaughan (1995, CIS*2520 revision 2006)
 * ------------------------------------------------------------------
 */

#include <stdlib.h>
#include <stdio.h>
#include "Element.h"


/*
 * ---------------------------------------------------------
 *	elementCreate
 *		- Creates new element, initializing contents to
 *		  be a reference to the provided data
 * ---------------------------------------------------------
 *	PRE:
 *		n/a
 * ---------------------------------------------------------
 *  POST:
 *		new element is allocated and initialized; returns a
 *		reference to the new element
 * ---------------------------------------------------------
 *	ERRORS:
 *		NULL : memory allocation failure (allocation)
 * ---------------------------------------------------------
 */
Element *elementCreate(void *data)
{
	Element *new;		/* new element */

	if (!(new = calloc(1,sizeof(Element))))
		return(NULL);

	new->data = data;

	return(new);

} /* elementCreate */



/*
 * ---------------------------------------------------------
 *	elementDelete
 *		- De-allocate memory associated with element
 *		  provided
 * ---------------------------------------------------------
 *	PRE:
 *		void
 * ---------------------------------------------------------
 *  POST:
 *		this element is destroyed; returns 0 if this was
 *		NULL, 1 otherwise
 * ---------------------------------------------------------
 *	ERRORS:
 *		void
 * ---------------------------------------------------------
 */
int elementDelete(Element *this)
{
	if (this)
	{
		free(this);
		return(1);
	}
	else
		return(0);

} /* elementDelete */



/*
 * ---------------------------------------------------------
 *	elementGetData
 *		- Obtain reference to the data stored in provided
 *		  element
 * ---------------------------------------------------------
 *	PRE:
 *		void
 * ---------------------------------------------------------
 *  POST:
 *		returns value of data
 * ---------------------------------------------------------
 *	ERRORS:
 *		void
 * ---------------------------------------------------------
 */
void *elementGetData(Element *this)
{
	return(this->data);

} /* elementGetData */


/*
 * ---------------------------------------------------------
 *	elementSetData
 *		- Reassign reference to the data stored in provided
 *		  element
 * ---------------------------------------------------------
 *	PRE:
 *		void
 * ---------------------------------------------------------
 *  POST:
 *		returns previous (overwritten) value of data
 * ---------------------------------------------------------
 *	ERRORS:
 *		void
 * ---------------------------------------------------------
 */
void *elementSetData(Element *this, void *data)
{
	void *save;
	
	save = this->data;
	this->data = data;

	return(save);

} /* elementSetData */



/*
 * ---------------------------------------------------------
 *	elementGetLeft
 *		- Obtain left reference of the provided element
 * ---------------------------------------------------------
 *	PRE:
 *		void
 * ---------------------------------------------------------
 *  POST:
 *		returns value of left
 * ---------------------------------------------------------
 *	ERRORS:
 *		void
 * ---------------------------------------------------------
 */
Element *elementGetLeft(Element *this)
{
	return(this->left);

} /* elementGetLeft */



/*
 * ---------------------------------------------------------
 *	elementGetRight
 *		- Obtain right reference of the provided element
 * ---------------------------------------------------------
 *	PRE:
 *		void
 * ---------------------------------------------------------
 *  POST:
 *		returns value of right
 * ---------------------------------------------------------
 *	ERRORS:
 *		void
 * ---------------------------------------------------------
 */
Element *elementGetRight(Element *this)
{
	return(this->right);

} /* elementGetRight */



/*
 * ---------------------------------------------------------
 *	elementSetLeft
 *		- Set left element reference of provided element
 * ---------------------------------------------------------
 *	PRE:
 *		void
 * ---------------------------------------------------------
 *  POST:
 *		left is assigned provided reference
 * ---------------------------------------------------------
 *	ERRORS:
 *		void
 * ---------------------------------------------------------
 */
void elementSetLeft(Element *this, Element *ref)
{
	this->left = ref;

} /* elementSetLeft */



/*
 * ---------------------------------------------------------
 *	elementSetRight
 *		- Set right element reference of provided element
 * ---------------------------------------------------------
 *	PRE:
 *		void
 * ---------------------------------------------------------
 *  POST:
 *		right is assigned provided reference
 * ---------------------------------------------------------
 *	ERRORS:
 *		void
 * ---------------------------------------------------------
 */
void elementSetRight(Element *this, Element *ref)
{
	this->right = ref;

} /* elementSetRight */
