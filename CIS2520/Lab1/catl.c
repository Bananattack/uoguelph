#include <stdio.h>
#include <string.h>

int main()
{
	char buffer[4096];

	while (!feof(stdin))
	{
		fgets(buffer, 4096, stdin);
		if (strlen(buffer) > 80)
		{
			printf("%s", buffer);
		}
	}

	return 0;
}
