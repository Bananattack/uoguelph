#include <stdio.h>

#define MAX_STRING_COUNT 10
#define MAX_STRING_LENGTH 41

void PrintStringsInReverseOrder(char string[MAX_STRING_COUNT][MAX_STRING_LENGTH], int count)
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
	char string[MAX_STRING_COUNT][MAX_STRING_LENGTH];

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

	for (i = 0; i < count; i++)
	{
		fscanf(f, "%40s", string[i]);
	}

	PrintStringsInReverseOrder(string, count);


	return 0;
}
