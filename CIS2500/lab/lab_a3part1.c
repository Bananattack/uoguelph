#include <stdio.h>
#include <stdlib.h>

void PrintSums(int **data, int rows, int columns)
{
	int i, j;
	int sum;
	for (i = 0; i < columns; i++)
	{
		sum = 0;
		for (j = 0; j < rows; j++)
		{
			sum += data[j][i];
		}
		printf("%d ", sum);
	}
}

int main()
{
	int i, j, rows, columns;
	int **data;

	FILE *f = fopen("input.txt", "r");
	if (!f)
	{
		return -1;
	}
	
	fscanf(f, "%d", &rows);
	fscanf(f, "%d", &columns);
	if (rows < 0)
	{
		return -1;
	}
	if (columns < 0)
	{
		return -1;
	}
	data = malloc(sizeof(int*) * rows);

	for (i = 0; i < rows; i++)
	{
		data[i] = malloc(sizeof(int) * columns);
		for (j = 0; j < columns; j++)
		{
			fscanf(f, "%d", &(data[i][j]));
		}
	}

	PrintSums(data, rows, columns);

	for (i = 0; i < rows; i++)
	{
		free(data[i]);
	}
	free(data);
	return 0;
}
