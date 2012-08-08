#include <stdio.h>
#include <stdbool.h>

int main()
{
	bool comment = false;
	bool string_literal = false;
	bool character_literal = false;
	char c;

	do
	{
		c = fgetc(stdin);
		if (c == '/' && !comment && !string_literal && !character_literal)
		{
			c = fgetc(stdin);
			if (c == '*')
			{
				comment = true;
			}
			else
			{
				printf("/%c", c);
			}
			continue;
		}
		if (c == '*' && comment)
		{
			c = fgetc(stdin);
			if (c == '/')
			{
				comment = false;
			}
			continue;
		}
		if (c == '\"' && !comment && !character_literal)
		{
			string_literal = !string_literal;
		}
		if (c == '\'' && !comment && !string_literal)
		{
			character_literal = !character_literal;
		}
		if (c != EOF && !comment)
		{
			printf("%c", c);
		}
	} while(c != EOF);

	return 0;
}
