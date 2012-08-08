/* External dependencies. */
#include <stdio.h>
#include <string.h>
#include "adt/CList.h"
#include "adt/Stack.h"

/*
	Each parenthesis has a line position
	and character position in its file.
*/
typedef struct Paren_t
{
	int line_offset;
	int character_offset;
} Paren;

/*
	Creates a parenthesis object.
*/
Paren* ParenNew(int line_offset, int character_offset)
{
	Paren* this = malloc(sizeof(Paren));
	this->line_offset = line_offset;
	this->character_offset = character_offset;
	return this;
}

/*
	Creates a string variable in dynamic memory.
*/
char* StringElementNew(char *value)
{
	char *s = malloc(sizeof(char) * (strlen(value) + 1));
	strcpy(s, value);
	return s;
}

/*
	The commandline help if no filename is passed.
*/
void PrintUsage(char* command)
{
	printf("____________________________________________________\n\n");
	printf("Usage:\t%s file\n", command);
	printf("Parses a file for matching parentheses.\n");
	printf("____________________________________________________\n\n");
}

/*
	A useful function to add at the end of a 
*/
void UtilityListAppend(List* l, void* s)
{
	listTail(l);
	listAddAfter(l, s);
}

/*
	Reads a bunch of lines from a file and then put it in a list of strings.
*/
List* FileGetLines(char* filename)
{
	FILE *f;
	char buffer[4096];
	List* lines;

	f = fopen(filename, "r");
	if(!f)
	{
		return NULL;
	}

	lines = listCreate();
	while(fgets(buffer, 4096, f))
	{
		UtilityListAppend(lines, StringElementNew(buffer));
	}
	fclose(f);

	return lines;
}

/*
	Prints a nice fancy error message with a line snippet and
	cursor pointing at the violating character.
*/
void ParenError(char* snippet, char* message, int line_offset, int char_offset)
{
	int i;
	printf("%s", snippet);
	for(i = 0; i < char_offset; i++)
	{
		printf(" ");
	}
	printf("^");
	printf("\n%s on line %d, column %d\n", message, line_offset + 1, char_offset + 1);
}

/*
	Given a list of text lines, scan it and match round brackets inside it.
*/
void ParenMatch(List* lines)
{
	int i;
	Stack* parens;
	Paren* p;
	char* s;
	int line_current, char_current = 0;
	int errors = 0;

	parens = stackCreate();

	listHead(lines);
	do
	{
		s = listGetCurrent(lines);

		char_current = 0;
		for (i = 0; i < strlen(s); i++)
		{
			/* Opening ( push require a new match. */
			if(s[i] == '(')
			{
				stackPush(parens, ParenNew(line_current, char_current));
			}
			/* Closing ) pop a match off. */
			if(s[i] == ')')
			{
				p = stackPop(parens);
				/* Nothing to close, too many closing parens. */
				if(p == NULL)
				{
					ParenError(s, "Unexpected closing )", line_current, char_current);
					errors++;
				}
				else
				{
					/*free(p);*/
				}
			}
			/* Tabs are interpreted as 8 spaces deep! */
			if(s[i] == '\t')
			{
				char_current += 8;
			}
			/* Blah, a normal character. */
			else
			{
				char_current++;
			}
		}
		line_current++;
	} while (listNext(lines));

	/* Pop off and catch all the unclosed parens. */
	while((p = stackPop(parens)))
	{
		listMoveToNth(lines, p->line_offset + 1);
		s = listGetCurrent(lines);
		ParenError(s, "No match for opening (", p->line_offset, p->character_offset);
		errors++;
		/*free(p);*/
	}

	stackDelete(parens);
	if(!errors)
	{
		printf("Parsed successfully.\n");
	}
	else
	{
		printf("Parse failed with %d matching error(s).\n", errors);
	}
}

int main(int argc, char** argv)
{
	char* filename;
	List* lines;

	/* Specified a file, open it. */
	if(argc > 1)
	{
		filename = argv[1];
		lines = FileGetLines(filename);
		if (lines)
		{
			ParenMatch(lines);
			/*listDelete(lines);*/
		}
		else
		{
			printf("File '%s' was not found!\n", filename);
		}
	}
	/* No parameters, print the program commandline help. */
	else
	{
		PrintUsage(argv[0]);
	}
	return 0;
}
