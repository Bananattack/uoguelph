#include <stdio.h>
#include <string.h>
#include <stdlib.h>
void PrintStringsInReverseOrder(char **string, int count)
{
	int i;
	for (i = count - 1; i >= 0; i--)
	{
		printf("%s\n", string[i]);
	}
}

int main()
{
	int i, count;
	char **string;
	char buffer[4096];

	FILE *f = fopen("input.txt", "r");
	if (!f)
	{
		return -1;
	}
	
	fscanf(f, "%d", &count);
	if (count > 10)
	{
		count = 10;
	}
	
	string = malloc(sizeof(char*) * count);

	for (i = 0; i < count; i++)
	{
		fscanf(f, "%s", buffer);
		/* Needs the space for a \0 as well. */
		string[i] = malloc(sizeof(char) * (strlen(buffer) + 1));
		strcpy(string[i], buffer);
	}

	PrintStringsInReverseOrder(string, count);


	return 0;
}
