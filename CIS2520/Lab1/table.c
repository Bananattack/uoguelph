#include <stdio.h>
#include <math.h>

int main()
{
	int i;

	printf("x	x^2	x^3	sqrt(x)\n");

	for (i = 0; i <= 100; i++)
	{
		printf("%d	%d	%d	%d\n", i, i * i, i * i * i, (int) sqrt(i));
	}
	return 0;
}
