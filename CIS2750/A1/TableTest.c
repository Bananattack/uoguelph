#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "adt/Table.h"

int StringEquals(void* ls, void* rs)
{
	return !strcmp(ls, rs);
}

#define STRING_HASH_MAX_LENGTH 16

int StringHash(void* key)
{
	int i = 0;
	int hash = 0;
	char* s = key;
	int len = STRING_HASH_MAX_LENGTH;

	len = (len < strlen(s)) ? len : strlen(s);

	while(i < len)
	{
		hash += (int) s[i];
		i++;
	}
	return hash;
}

char* StringNew(char *value)
{
	char *s = (char *) malloc(sizeof(char) * (strlen(value) + 1));
	strcpy(s, value);
	return s;
}

void StringFree(char* value)
{
	free(value);
}

int main()
{
	Table* table = tableCreate(50, StringHash, StringEquals);
	if(!table)
	{
		printf("Table failed allocation. Whoops!\n");
		return -1;
	}
	tableAddEntry(table, "blah", StringNew("boo"));
	printf("%s\n", (char*) tableGetEntry(table, "blah"));
	StringFree(tableGetEntry(table, "blah"));
	return 0;
}

