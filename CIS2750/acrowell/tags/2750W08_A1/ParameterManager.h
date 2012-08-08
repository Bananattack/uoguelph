#ifndef PMANAGER_H
#define PMANAGER_H

#include <stdlib.h>
#include "adt/Table.h"
#include "adt/String.h"
#include "adt/Boolean.h"
#include "ParameterList.h"

#define PARAM_MAX_IDENTIFER_LENGTH 4096
/* ParameterManager ADT */
typedef enum { INT_TYPE, REAL_TYPE, BOOLEAN_TYPE, STRING_TYPE, LIST_TYPE } param_t;

union param_value
{
    int int_val;
    float real_val;
    Boolean bool_val;
    char* str_val;
    ParameterList* list_val;
};

typedef struct ParameterValue_t
{
	/* Denotes whether the value for a parameter was set */
	int initialized;
	/* The type of parameter to expect. Parse error on invalid values */
	param_t type;
	/* The union data that needs to be returned */
	union param_value value;
} ParameterValue;

typedef struct ParameterManager_t
{
	/* A table that maps a char* to its ParamaterValue* information */
	Table* values;
	/* Keeps tabs on which table entries must be defined. */
	List* required;
} ParameterManager;

ParameterManager* PM_create(int size);
int PM_destroy(ParameterManager* this);
int PM_hasInitalizedAllRequired();
int PM_parseFrom(ParameterManager* this, FILE* f, char comment_char);
int PM_manage(ParameterManager* this, char* name, param_t type, int required);
int PM_hasValue(ParameterManager* this, char* name);
union param_value PM_getValue(ParameterManager* this, char *name);

#endif
