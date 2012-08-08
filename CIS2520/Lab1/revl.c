#include <stdio.h>
#include <string.h>
#include <math.h>

int max(int a, int b)
{
	return a > b ? a : b;
}

/* In-place reversal of a string. */
void reverse(char* s)
{
	int i, j;
	char t;
	int len = strlen(s);
	for (i = 0; i < len / 2; i++)
	{
		j = len - i - 1;

		t = s[i];
		s[i] = s[j];
		s[j] = t;
	}
}

int main()
{
	char buffer[4096];

	while (!feof(stdin))
	{
		fgets(buffer, 4096, stdin);

		/* Remove the newline, so it doesn't get reversed. */
		if (buffer[max(strlen(buffer) - 1, 0)] == '\n')
		{
			buffer[max(strlen(buffer) - 1, 0)] = '\0';
		}

		reverse(buffer);
		printf("%s\n", buffer);
	}

	return 0;
}
