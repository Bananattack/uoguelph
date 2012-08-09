/*
	CIS*2750
	Andrew G. Crowell
	0545826
	acrowell@uoguelph.ca
*/
#ifndef TABLE_H
#define TABLE_H

#include <stdlib.h>
#include <stdio.h>
#include "Boolean.h"
#include "List.h"

typedef struct TableNode_t
{
	void* key;
	void* data;
} TableNode;

typedef FILE OutputStream;
typedef int (*HashGenerator)(void *);
typedef int (*EqualityComparator)(void *, void*);

typedef struct Table_t
{
	int size;
	int entry_count;
	List** lookup;
	HashGenerator hash_generator;
	EqualityComparator comparator;
} Table;

Table* tableCreate(int size, HashGenerator hash_generator, EqualityComparator comp);
void tableDelete(Table* this);
void tableAddEntry(Table* this, void* key, void* value);
void* tableRemoveEntry(Table* this, void* key);
void* tableGetEntry(Table* this, void* key);
int tableSize(Table* this);
int tableEntries(Table* this);
double tableLoad(Table* this);

#endif
