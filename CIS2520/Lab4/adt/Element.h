/*
 * ------------------------------------------------------------------
 *	Element.h - Element ADT prototypes/data declarations
 *
 *	Author: David McCaughan (1995, CIS2520 revision 2006)
 * ------------------------------------------------------------------
 */

#ifndef ELEMENT_H
#define ELEMENT_H


/*
 * --- TYPE: Element -------------------------------------------------------
 *	Component element in 2-way linked data structures
 * -------------------------------------------------------------------------
 */
typedef struct Element_t
{
	void             *data;		/* reference to data */
	struct Element_t *left;		/* reference to previous/left element */
	struct Element_t *right;	/* reference to next/right element */
} Element;


/* --- file: Element.c --- */

Element * elementCreate   (void *);
int       elementDelete   (Element *);
void    * elementGetData  (Element *);
void    * elementSetData  (Element *, void *);
Element * elementGetLeft  (Element *);
Element * elementGetRight (Element *);
void      elementSetLeft  (Element *, Element *);
void      elementSetRight (Element *, Element *);


#endif
