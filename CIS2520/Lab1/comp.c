#include <stdio.h>

int main()
{
	char character_last, character_current = -1;
	int character_count = 1;

	do
	{
		character_last = character_current;
		character_current = fgetc(stdin);
		if (character_current == character_last)
		{
			character_count++;
			if (character_count == 9)
			{
				printf("%d", character_count);	
				printf("%c", character_current);
				character_count = 0;
			}
		}
		else if (character_current != EOF)
		{
			if (character_count > 1)
			{
				printf("%d", character_count);	
			}
			printf("%c", character_current);
			character_count = 1;
		}
	} while(character_current != EOF);

	return 0;
}
