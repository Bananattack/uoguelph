#include "String.h"

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
