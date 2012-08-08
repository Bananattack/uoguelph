#include <stdio.h>
#include <stdlib.h>

struct matrix
{
	int ht, wd;
	float **A;
};

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


int main(int argc, char *argv[])
{
	struct matrix *m, *n, *result;

	/* create structure and fill array */
	m = createMatrix(3, 2);

	m->A[0][0] = 1;
	m->A[0][1] = 2;
	m->A[1][0] = 3;
	m->A[1][1] = 4;
	m->A[2][0] = 5;
	m->A[2][1] = 6;

	printf("First Matrix\n");
	printMatrix(m);

	/* create a second structure and fill array */
	n = createMatrix(3, 2);

	n->A[0][0] = 6;
	n->A[0][1] = 5;
	n->A[1][0] = 4;
	n->A[1][1] = 3;
	n->A[2][0] = 2;
	n->A[2][1] = 1;

	printf("Second Matrix\n");
	printMatrix(n);

	/* call add function and print results */
	result = add(m, n);

	printf("Add result\n");
	printMatrix(result);

	freeMatrix(result);

	/* call multiply function and print results */
	result = multiply(m, n);

	printf("Multiply result\n");
	printMatrix(result);

	freeMatrix(result);

	freeMatrix(m);
	freeMatrix(n);

	return 0;
}
