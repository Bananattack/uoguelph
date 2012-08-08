#include <stdio.h>
#define MAX_ROWS 50
#define MAX_COLUMNS 50

void PrintSums(int data[MAX_ROWS][MAX_COLUMNS], int rows, int columns)
{
	int i, j;
	int sum[MAX_COLUMNS];
	for (i = 0; i < columns; i++)
	{
		sum[i] = 0;
		for (j = 0; j < rows; j++)
		{
			sum[i] += data[j][i];
		}
		if (i == columns - 1)
		{
			printf("%d\n", sum[i]);
		}
		else
		{
			printf("%d ", sum[i]);
		}
	}
}

int main()
{
	int i, j, rows, columns;
	int data[MAX_ROWS][MAX_COLUMNS];

	FILE *f = fopen("input.txt", "r");
	if (!f)
	{
		return -1;
	}
	
	fscanf(f, "%d", &rows);
	fscanf(f, "%d", &columns);
	if (rows < 0 || rows >= MAX_ROWS)
	{
		return -1;
	}
	if (columns < 0 || columns >= MAX_COLUMNS)
	{
		return -1;
	}

	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < columns; j++)
		{
			fscanf(f, "%d", &(data[i][j]));
		}
	}

	PrintSums(data, rows, columns);

	return 0;
}
