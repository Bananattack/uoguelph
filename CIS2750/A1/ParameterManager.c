#include "ParameterManager.h"

/*
	Creates a new parameter manager object
	PRE: size is a positive integer
	POST:	Returns a new parameter manager object initialized to be empty
		(i.e. managing no parameters) on success,
		NULL otherwise (memory allocation failure)
*/
ParameterManager* PM_create(int size)
{
	ParameterManager* this = malloc(sizeof(ParameterManager));
	if (!this || size <= 0)
	{
		return NULL;
	}
	else
	{
		this->values = tableCreate(size, StringHash, StringEquals);
		if (!this->values)
		{
			free(this);
			return NULL;
		}

		this->required = listCreate();
		if (!this->required)
		{
			free(this->values);
			free(this);
			return NULL;
		}
	}
	return this;
}

/*
	Destroys a parameter manager object
	PRE: n/a
	POST:	all memory associated with the parameter manager is freed;
		returns 1 on success,
		0 otherwise
*/
int PM_destroy(ParameterManager* this)
{
	tableDelete(this->values);
	listDelete(this->required);
	free(this);
	return 0;
}

/*
	Check that all required parameters have been initialized
	PRE: none
	POST:	Returns 1 if all required parameters are initialized,
		0 otherwise.

*/
int PM_hasInitializedAllRequired(ParameterManager* this)
{
	List* iterator;
	char* current;
	iterator = listHead(this->required);
	current = listGetCurrent(this->required);
	while (iterator != NULL)
	{
		if(!(((ParameterValue*) tableGetEntry(this->values, current))->initialized))
		{
			return 0;
		}
		iterator = listNext(this->required);
		current = listGetCurrent(this->required);
	}
	return 1;
}

Boolean __parseIntegerLiteral(char* value, int* val)
{
	int i;
	for (i = 0; i < strlen(value); i++)
	{
		if((value[i] >= '0' && value[i] <= '9') || (value[i] == '-' && i == 0))
		{
			continue;
		}
		else
		{
			return false;
		}
	}
	sscanf(value, "%d", val);
	return true;
}

Boolean __parseRealLiteral(char* value, float* val)
{
	int i;
	int decimal_point = 0;
	int trailing_num = 0;

	for (i = 0; i < strlen(value); i++)
	{
		if ((value[i] >= '0' && value[i] <= '9') || (value[i] == '-' && i == 0))
		{
			if(decimal_point)
			{
				trailing_num = true;
			}
		}
		else if (value[i] == '.')
		{
			/* Already has a decimal point, got something like 5.5.436, which is invalid. */
			if(decimal_point)
			{
				return false;
			}
			/* Found the (optional) decimal point, keep note of it.*/
			else
			{
				decimal_point = 1;
			}
		}
		else
		{
			return false;
		}
	}
	/* Error for stupid decimals like '5.' instead of '5.0' or '5', where decimal points aren't necessary. */
	if(decimal_point && !trailing_num)
	{
		return false;
	}
	sscanf(value, "%f", val);
	return true;
}

Boolean __parseBooleanLiteral(char* value, Boolean* val)
{
	int i;
	if(!strcmp(value, "true") || !strcmp(value, "false"))
	{
		*val = (!strcmp(value, "true")) ? true : false;
		return true;
	}
	return false;
}

Boolean __parseListLiteral(char* value, ParameterList** val)
{
	int i, pos;
	char buffer[PARAM_MAX_IDENTIFER_LENGTH];
	Queue* q = queueCreate();
	ParameterList* pl = NULL;

	/* Couldn't allocate new queue memory */
	if(!q)
	{
		return false;
	}

	/* No opening bracket */
	if (value[0] != '{')
	{
		return false;
	}
	/* No closing bracket */
	if (value[strlen(value) - 1] != '}')
	{
		return false;
	}

	for (i = 1, pos = 0; i < strlen(value) - 1; i++)
	{
		/* If a comma is read in, then advance to the next string*/
		if (value[i] == ',')
		{
			buffer[pos] = '\0';
			/* Empty strings are baaaad. */
			if (!pos)
			{
				queueDelete(q);
				return false;
			}
			queueArrive(q, StringNew(buffer));			
			pos = 0;
		}
		/* Abuse of the list delimiters. No such thing as a nested list. */
		else if (value[i] == '{' || value[i] == '}')
		{
			queueDelete(q);
			return false;
		}
		/* Otherwise read in the string as normal. */
		else
		{
			buffer[pos++] = value[i];
		}
	}
	/* Last string in the list */
	if (pos)
	{
		buffer[pos] = '\0';
		queueArrive(q, StringNew(buffer));
	}
	*val = PL_create(q);
	if(!*val)
	{
		queueDelete(q);
		return false;
	}
	return true;
}

/*
	Extract values for parameters from an input stream
	PRE: file passed is a valid input stream ready for reading that contains the desired parameters
	POST:	All required parameters, and those optional parameters present,
		are assigned values that are consumed from the file given,
		respecting comment as a "start of comment to end of line"
		character if not nul ('\0');
		returns non-zero on success,
		0 otherwise (parse error, memory allocation failure)
*/
int PM_parseFrom(ParameterManager* this, FILE* f, char comment_char)
{
	char c;
	Boolean has_key = false;
	Boolean in_comment = false;
	int i;
	char k[PARAM_MAX_IDENTIFER_LENGTH], v[PARAM_MAX_IDENTIFER_LENGTH];
	ParameterValue* pv;

	if (!f)
	{
		return 0;
	}
	/* TODO: Make the parser */
	while ((c = (char) fgetc(f)) != EOF)
	{

		/* Ignore whitespace */
		if (c == ' ' || c == '\t')
		{
			continue;
		}
		/* Newline. Kills comments, could increase line number when this information is relevant */
		else if (c == '\n')
		{
			in_comment = false;
		}
		else if(!in_comment)
		{
			if(c == comment_char)
			{
				in_comment = true;
			}
			else if (c == '=')
			{
				if (!has_key && i > 0)
				{
					v[i] = '\0';
					strcpy(k, v);
					has_key = true;
					i = 0;
				}
				/* Unexpected equals, cannot chain assignment. */
				else
				{
					return 0;
				}
			}
			else if (c == ';')
			{
				if(has_key && i > 0)
				{
					v[i] = '\0';
					pv = tableGetEntry(this->values, k);
					/*
						No parameter associated with that name,
						or already initialized that parameter
					*/
					if(!pv || pv->initialized)
					{
						return 0;
					}
					switch(pv->type)
					{
						case STRING_TYPE:
							pv->value.str_val = StringNew(v);
							break;
						case INT_TYPE:
							if(!__parseIntegerLiteral(v, &(pv->value.int_val)))
							{
								return 0;
							}
							break;
						case BOOLEAN_TYPE:
							if(!__parseBooleanLiteral(v, &(pv->value.bool_val)))
							{
								return 0;
							}
							break;
						case REAL_TYPE:
							if(!__parseRealLiteral(v, &(pv->value.real_val)))
							{
								return 0;
							}
							break;
						case LIST_TYPE:
							if(!__parseListLiteral(v, &(pv->value.list_val)))
							{
								return 0;
							}
							break;
					}
					pv->initialized = 1;
					has_key = false;
					i = 0;
				}
				/* Unexpected semi-colon */
				else
				{
					return 0;
				}
			}
			else
			{
				if (i < PARAM_MAX_IDENTIFER_LENGTH - 1)
				{
					v[i++] = c;
				}
				/* Delimiter is too long. */
				else
				{
					return 0;
				}
			}
		}
	}
	/* Unexpected eof. */
	if(has_key || i > 0)
	{
		return 0;
	}
	return PM_hasInitializedAllRequired(this);
}

/*
	Register parameter for management
	PRE: name does not duplicate the name of a parameter already managed
	POST:	Parameter named name of parameter type type is registered with
		this manager as a parameter;
		if required is zero the parameter will be considered optional,
		otherwise it will be considered required; returns 1 on success,
		0 otherwise (duplicate name, memory allocation failure)
*/
int PM_manage(ParameterManager* this, char* name, param_t type, int required)
{
	/*
		Names must be copies of the string memory,
		so we don't have to worry about how
		the user is managing them outside of this module.
	*/
	char* k;
	ParameterValue* v;
	char* required_name;
	
	/* Duplicate name, whuh oh. */
	if (tableGetEntry(this->values, name))
	{
		return 0;
	}
	
	k = StringNew(name);
	if (!k)
	{
		return 0;
	}

	v = malloc(sizeof(ParameterValue));
	v->initialized = 0;
	v->type = type;
	if (!v)
	{
		free(k);
		return 0;
	}

	tableAddEntry(this->values, k, v);
	
	/* Required strings need to be noted as such. Add it to the list */
	if (required)
	{
		required_name = StringNew(name);
		listTail(this->required);
		listAddAfter(this->required, required_name);
		if (!required_name)
		{
			free(k);
			free(v);
		}
	}

	/* Voila, a new parameter is now managed! */
	return 1;
}

/*
	Test if a parameter has been assigned a value
	PRE: name is the name of a parameter currently managed by this manager
	POST:	Returns 1 if pname has been assigned a value,
		0 otherwise (no value, unknown parameter)
*/
int PM_hasValue(ParameterManager* this, char* name)
{
	ParameterValue* v = tableGetEntry(this->values, name);
	if (v && v->initialized)
	{
		return 1;
	}
	return 0;
}

/*
	Obtain the value assigned with a parameter of a specified name
	PRE:	the name must refer to a parameter currently managed by this manager
		and has been assigned a value
	POST:	Returns the value assigned to a the parameter of the specified name;
		result is undefined if it has not been assigned a value or is unknown
*/
union param_value PM_getValue(ParameterManager* this, char *name)
{
	ParameterValue* v = tableGetEntry(this->values, name);
	if (v && v->initialized)
	{
		return v->value;
	}
}








