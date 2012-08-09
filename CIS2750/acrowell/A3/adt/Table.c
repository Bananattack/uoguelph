/*
	CIS*2750
	Andrew G. Crowell
	0545826
	acrowell@uoguelph.ca
*/
#include "Table.h"

/*
 * ---------------------------------------------------------
 *	tableCreate
 *		- Create new table initialized to empty,
 *		  with the given hash generator and
 *		  equality comparator callback functions.
 * ---------------------------------------------------------
 *	PRE:
 *		none
 * ---------------------------------------------------------
 *	POST:
 *		new table is allocated and initialized; returns a
 *		reference to new table
 * ---------------------------------------------------------
 *	ERRORS:
 *		NULL : memory allocation failure (allocation)
 * ---------------------------------------------------------
 */
Table* tableCreate(int size, HashGenerator hash_generator, EqualityComparator comparator)
{
	int i, j;
	Table* this = malloc(sizeof(Table));
	if (this == NULL)
	{
		return NULL;
	}

	this->lookup = malloc(sizeof(List*) * size);
	if(this->lookup == NULL)
	{
		free(this);
		return NULL;
	}
	for(i = 0; i < size; i++)
	{
		this->lookup[i] = listCreate();
		if(this->lookup[i] == NULL)
		{
			for(j = 0; j < i; j++)
			{
				listDelete(this->lookup[j]);
			}
			free(this->lookup);
			free(this);
			return NULL;
		}
	}

	this->size = size;
	this->entry_count = 0;
	this->hash_generator = hash_generator;
	this->comparator = comparator;

	return this;
}

/*
 * ---------------------------------------------------------
 *	tableDelete
 *		- De-allocate all memory associated with provided
 *		  table object
 * ---------------------------------------------------------
 *	PRE:
 *		none
 * ---------------------------------------------------------
 *	POST:
 *		this table is destroyed; returns number of elements
 *		that were in the table
 * ---------------------------------------------------------
 */
void tableDelete(Table* this)
{
	int i;
	for(i = 0; i < this->size; i++)
	{
		listDelete(this->lookup[i]);
	}
	free(this->lookup);
	free(this);
}

/*
 * ---------------------------------------------------------
 *	tableAddEntry
 *		- adds an entry into the hash table, which
 *		  effectively maps the key to a value.
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
 */
void tableAddEntry(Table* this, void* key, void* value)
{
	int i;
	TableNode* node = NULL;
	int index = this->hash_generator(key) % this->size;
	bool found = false;

	listHead(this->lookup[index]);
	/* Loop until we either find the entry or reach the end of the list. */
	do 
	{
		node = (TableNode*) listGetCurrent(this->lookup[index]);
		if(node != NULL && this->comparator(node->key, key))
		{
			found = true;
			break;
		}
	} while(listNext(this->lookup[index]) != NULL);

	/* If we didn't find the key earlier, make a new node with this key */
	if(!found)
	{		
		node = malloc(sizeof(node));
		node->key = key;
		node->data = value;
		listAddAfter(this->lookup[index], node);
		this->entry_count++;
	}
	else
	{
		fprintf(stderr, "\n\tERROR: Attempting to overwrite existing key.\n");
		exit(-1);	
	}
}

/*
 * ---------------------------------------------------------
 *	tableRemoveEntry
 *		- Remove data associated with a given table key
 *		  and obtain a reference to the data in the element
 * ---------------------------------------------------------
 *	PRE:
 *		void
 * ---------------------------------------------------------
 *	POST:
 *		returns a reference to the data at the key
 *		in this table which no longer exists in
 *		the table; Association entry is destroyed.
 * ---------------------------------------------------------
 *	ERRORS:
 *		NULL : entry associated with key not found
 * ---------------------------------------------------------
 */
void* tableRemoveEntry(Table* this, void* key)
{
	int index = this->hash_generator(key) % this->size;
	void* result = NULL;
	TableNode* node = NULL;
	bool found = false;

	listHead(this->lookup[index]);
	/* Loop until we either find the entry or reach the end of the list. */
	do 
	{
		node = (TableNode*) listGetCurrent(this->lookup[index]);
		if(node != NULL && this->comparator(node->key, key))
		{
			found = true;
			break;
		}
	} while(listNext(this->lookup[index]) != NULL);

	/* Found it, delete it */
	if(found)
	{
		result = node->data;

		free(node);
		listDelCurrent(this->lookup[index]);
		this->entry_count--;
	}
	return result;
}

/*
 * ---------------------------------------------------------
 *	tableGetEntry
 *		- Get value reference associated with a given table key
 * ---------------------------------------------------------
 *	PRE:
 *		void
 * ---------------------------------------------------------
 *	POST:
 *		returns a reference to the data at the key
 * ---------------------------------------------------------
 *	ERRORS:
 *		NULL : entry associated with key not found
 * ---------------------------------------------------------
 */
void* tableGetEntry(Table* this, void* key)
{
	int index = this->hash_generator(key) % this->size;
	TableNode* node = NULL;
	bool found = false;

	listHead(this->lookup[index]);
	/* Loop until we either find the entry or reach the end of the list. */
	do 
	{
		node = (TableNode*) listGetCurrent(this->lookup[index]);
		if(node != NULL && this->comparator(node->key, key))
		{
			found = true;
			break;
		}
	} while(listNext(this->lookup[index]) != NULL);

	/* Found it */
	if(found)
	{
		return node->data;
	}
	return NULL;
}

/*
 * ---------------------------------------------------------
 *	tableSize
 *		- gets the physical table size
 * ---------------------------------------------------------
 *	PRE:
 *		void
 * ---------------------------------------------------------
 *	POST:
 *		returns the size
 * ---------------------------------------------------------
 */
int tableSize(Table* this)
{
	return this->size;
}

/*
 * ---------------------------------------------------------
 *	tableEntries
 *		- gets the number of entries actually stored
 * ---------------------------------------------------------
 *	PRE:
 *		void
 * ---------------------------------------------------------
 *	POST:
 *		returns the entry count
 * ---------------------------------------------------------
 */
int tableEntries(Table* this)
{
	return this->entry_count;
}

/*
 * ---------------------------------------------------------
 *	tableLoad
 *		- gets the load factor of the table.
 * ---------------------------------------------------------
 *	PRE:
 *		the size of the physical table isn't 0,
 *		or else division by zero occurs.
 *		But who the hell would want a zero-size table
 *		anyway? 
 * ---------------------------------------------------------
 *	POST:
 *		returns the load factor of table,
 *		which equals (entry count / physical size)
 * ---------------------------------------------------------
 */
double tableLoad(Table* this)
{
	return (double) this->entry_count / (double) this->size;
}
