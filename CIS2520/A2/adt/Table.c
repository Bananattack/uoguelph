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

	this->data = malloc(sizeof(void*) * size);
	if(this->data == NULL)
	{
		for(i = 0; i < size; i++)
		{
			listDelete(this->lookup[i]);
		}
		free(this->lookup);
		free(this);
		return NULL;
	}
	for(i = 0; i < size; i++)
	{
		this->data[i] = NULL;
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
	free(this->data);
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

	/* Find an empty space in the data array. */
	for(i = 0; i < this->size; i++)
	{
		/* Add the entry at the empty space */
		if(this->data[i] == NULL)
		{
			/* Set the data */
			this->data[i] = value;

			/* If we didn't find the key earlier, make a new node with this key */
			if(!found)
			{		
				node = malloc(sizeof(node));
				node->key = key;
				node->index = i;
				listAddAfter(this->lookup[index], node);
			}
			else
			{
				fprintf(stderr, "\n\tERROR: Attempting to overwrite existing key.\n");
				exit(-1);
			}
			this->entry_count++;
			return;
		}
	}
	fprintf(stderr, "\n\tERROR: Couldn't add key/value pair, because the data table is full.\n");
	fprintf(stderr, "\tYeah, this table doesn't resize itself dynamically,\n");
	fprintf(stderr, "\tso you're stuck with a load factor <= 1. Supersorry.\n");
	fprintf(stderr, "\tYou might want to give this table a bigger size instead?\n");
	exit(-1);	
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
		result = this->data[node->index];

		/* Make an empty space in the data array again. */
		this->data[node->index] = NULL;

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
		return this->data[node->index];
	}
	return NULL;
}

/*
 * ---------------------------------------------------------
 *	tableWrite
 *		- writes the table to an output stream
 *		- NOTE: This violates the generic data
 *		  interface because Dave gave permission
 *		  to write a printout that actually knows
 *		  the types associated with the key/value pairs.
 * ---------------------------------------------------------
 *	PRE:
 *		the output stream passed is a FILE* that
 *		has the "r" property.
 * ---------------------------------------------------------
 *	POST:
 *		the table representation is dumped to the stream.
 * ---------------------------------------------------------
 */
void tableWrite(Table* this, OutputStream* stream)
{
	int i;
	TableNode* node = NULL;
	for(i = 0; i < this->size; i++)
	{
		listHead(this->lookup[i]);
		if (!listIsEmpty(this->lookup[i]))
		{
			fprintf(stream, "Hash entry %d\n", i);
			do
			{
				node = (TableNode*) listGetCurrent(this->lookup[i]);
				if(node != NULL)
				{
					fprintf(stream, "\t%s: %d\n", (char*) node->key, *((int*) this->data[node->index]));
				}
			} while(listNext(this->lookup[i]) != NULL);
		}
	}
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
