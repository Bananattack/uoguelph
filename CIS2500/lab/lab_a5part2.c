#include <stdio.h>
#include <stdlib.h>

int main()
{
	int number[100];
	int a, i, sum;

	FILE *f = fopen("temp", "w");
	
	for (i = 0; i < 100; i++)
	{
		number[i] = rand();
		fprintf(f, "%d ", number[i]);
	}
	fclose(f);

	f = fopen("temp", "r");
	for (i = 0; i < 100; i++)
	{
		fscanf(f, "%d", &a);
		sum += i;
	}
	fclose(f);

	printf("The sum of 100 pseudorandom numbers: %d\n", sum);

	return 0;
}
