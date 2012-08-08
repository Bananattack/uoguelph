#include "matrix.h"

int __matrixMin(int a, int b)
{
	return (a > b) ? a : b;
}

struct matrix* createMatrix(int ht, int wd)
{
	int i, j;
	struct matrix *ptr = malloc(sizeof(struct matrix));
	if (ptr == NULL)
	{
		return NULL;
	}
	ptr->ht = ht;
	ptr->wd = wd;
	ptr->A = malloc(sizeof(float*) * ptr->ht);
	if (ptr->A == NULL)
	{
		free(ptr);
		return NULL;
	}
	for (i = 0; i < ptr->ht; i++)
	{
		ptr->A[i] = malloc(sizeof(float) * ptr->wd);
		if (ptr->A[i] == NULL)
		{
			for (j = 0; j < i; j++)
			{
				free(ptr->A[j]);
			}
			free(ptr->A);
			free(ptr);
			return NULL;
		}
	}
	return ptr;
}

struct matrix* add(struct matrix *ls, struct matrix *rs)
{
	int i, j;
	int ht, wd;
	struct matrix *result;
	if (ls == NULL || rs == NULL)
	{
		return NULL;
	}
	ht = __matrixMin(ls->ht, rs->ht);
	wd = __matrixMin(ls->wd, rs->wd);
	result = createMatrix(ht, wd);
	if (result == NULL)
	{
		return NULL;
	}
	for (i = 0; i < result->ht; i++)
	{
		for (j = 0; j < result->ht; j++)
		{
			result->A[i][j] = ls->A[i][j] + rs->A[i][j];
		}
	}
	return result;
}

struct matrix* multiply(struct matrix *ls, struct matrix *rs)
{
	int i, j;
	int ht, wd;
	struct matrix *result;
	if (ls == NULL || rs == NULL)
	{
		return NULL;
	}
	ht = __matrixMin(ls->ht, rs->ht);
	wd = __matrixMin(ls->wd, rs->wd);
	result = createMatrix(ht, wd);
	if (result == NULL)
	{
		return NULL;
	}
	for (i = 0; i < result->ht; i++)
	{
		for (j = 0; j < result->ht; j++)
		{
			result->A[i][j] = ls->A[i][j] * rs->A[i][j];
		}
	}
	return result;
}

/* Prints the contents of the array passed into the function */
void printMatrix(struct matrix *ptr)
{
	int i, j;
	for(i=0; i < ptr->ht; i++)
	{
		for(j=0; j < ptr->wd; j++)
		{
        		printf("%f ", ptr->A[i][j]);
        	}
		printf("\n");
        }
	printf("\n");
}

void freeMatrix(struct matrix *ptr)
{
	int i;
	for (i=0; i < ptr->ht; i++)
	{
		free(ptr->A[i]);
	}
	free(ptr->A);
	free(ptr);
}
