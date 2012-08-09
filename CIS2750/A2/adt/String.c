/*
	CIS*2750
	Andrew G. Crowell
	0545826
	acrowell@uoguelph.ca
*/
#include "String.h"

char* StringNew(const char* value)
{
	char *s = (char *) malloc(sizeof(char) * (strlen(value) + 1));
	strcpy(s, value);
	return s;
}

void StringFree(char* value)
{
	free(value);
}

int StringEquals(void* ls, void* rs)
{
	return !strcmp(ls, rs);
}

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
