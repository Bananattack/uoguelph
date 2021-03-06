/*
	CIS*2750
	Andrew G. Crowell
	0545826
	acrowell@uoguelph.ca
*/
#ifndef STRG_H
#define STRG_H

#include <string.h>
#include <stdlib.h>

#define STRING_HASH_MAX_LENGTH 16

char* StringNew(const char* value);
void StringFree(char* value);
int StringEquals(void* ls, void* rs);
int StringHash(void* key);

#endif
