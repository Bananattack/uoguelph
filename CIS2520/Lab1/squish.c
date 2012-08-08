#include <stdio.h>
#include <stdbool.h>
#include <string.h>

int main()
{
	bool space = false;
	char c;

	do
	{
		c = fgetc(stdin);
		if (c == ' ' || c == '\t')
		{
			if (!space)
			{
				space = true;
				printf(" ");
			}
			else if (space)
			{
				continue;
			}
		}
		else if (c != EOF)
		{
			printf("%c", c);
			space = false;
		}
	} while(c != EOF);

	return 0;
}
