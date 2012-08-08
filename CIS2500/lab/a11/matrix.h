#include <stdio.h>
#include <stdlib.h>

struct matrix
{
	int ht, wd;
	float **A;
};

int __matrixMin(int a, int b);

struct matrix* createMatrix(int ht, int wd);
struct matrix* add(struct matrix *ls, struct matrix *rs);
struct matrix* multiply(struct matrix *ls, struct matrix *rs);
void printMatrix(struct matrix *ptr);
void freeMatrix(struct matrix *ptr);
