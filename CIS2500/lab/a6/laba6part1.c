#include <stdio.h>

int main()
{
	int i;
	FILE *f;
	int array[100];
	int array2[100];

	f = fopen("temp", "w");
	for (i = 0; i < 100; i++)
	{
		array[i] = i + 1;
	}
	fwrite(array, sizeof(int), 100, f);
	fclose(f);

	f = fopen("temp", "r");
	fread(array2, sizeof(int), 100, f);
	for (i = 0; i < 100; i++)
	{
		printf("%d\n", array2[i]);
	}
	fclose(f);

	return 0;
}
