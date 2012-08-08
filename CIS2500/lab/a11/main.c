#include "matrix.h"

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
