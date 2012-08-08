#include <stdio.h>
#include <string.h>

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

/* Converts a number from base 10 to some other base and stores that in a string */
void itob(int value, int base, char *result)
{
	int i = 0;
	int digit;
	while (value)
	{
		digit = value % base;
		value /= base;

		if (digit >= 0 && digit <= 9)
		{
			result[i] = '0' + digit;
		}
		else if (digit >= 10 && digit <= 36)
		{
			result[i] = 'A' + (digit - 10);
		}
		else
		{
			break;
		}
		i++;
	}
	result[i] = '\0';

	/*
		The last digit calculated is the first in the string.
		Reverse!
	*/
	reverse(result);
}

int main()
{
	int num, base;
	char result[33];
	scanf("%d%d", &num, &base);
	itob(num, base, result);
	printf("%s\n", result);
	return 0;
}
