/*
	Assignment 1 - Dynamic Memory voodoo and such

	Andrew Crowell
	0545826

*/



/* External dependencies */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* The height and width of the ad-hoc 2D dimensional array. */
int ht, wd;

/*
	Allocates a 1D array that will be manipulated as a 2D array.

	Pass: number of rows, number of columns
	Return: dynamically allocated array.
	Assumes: Only one 2D array is ever in existance at any time.
*/
int* create2DArray(int rows, int columns)
{
	int *array = malloc(sizeof(int) * rows * columns);
	ht = rows;
	wd = columns;
	return array;
}

/*
	Sets an element in a 1D array as if it were in a 2D array.
	Emulates the behaviour of array[rowIndex][columnIndex] = value;
	
	Pass: the array to use, the row, the column, the value.
*/
void set2DElement(int *array, int rowIndex, int columnIndex, int value)
{
	array[rowIndex * ht + columnIndex] = value;
}

/*
	Sets an element in a 1D array as if it were in a 2D array.
	Emulates the behaviour of the expression array[rowIndex][columnIndex]
	
	Pass: the array to use, the row, the column.
	Return: the value at the specified row and column in the array.
*/
int get2DElement(int *array, int rowIndex, int columnIndex)
{
	return array[rowIndex * ht + columnIndex];
}

/*
	Frees up the memory used by the array.
*/
void free2DArray(int *array)
{
	free(array);
}

/*
	Creates an array of the specified size to store string pointers.
	All values are initialized to NULL.

	Pass: the size of the array
	Return: the string array.
*/
char** createStringArray(int size)
{
	int i;
	char **array = malloc(sizeof(char *) * size);
	for (i = 0; i < size; i++)
	{
		array[i] = NULL;
	}
	return array;
}

/*
	Sets a string in the array to a copy of the string value passed.

	Pass: the array, the index in the array, the string value to store
*/
void setStringArray(char **array, int index, char *value)
{
	/*
		strlen(value) + 1 gives the size
		of the string plus the \0 terminator.
	*/
	array[index] = malloc(sizeof(char) * (strlen(value) + 1));
	strcpy(array[index], value);
}

/*
	Gets a string out of the array

	Pass: the array, the index in the array
	Return: the value of the string stored at that index.
*/
char* getStringArray(char **array, int index)
{
	return array[index];
}

/*
	Frees up the memory used by the string array.

	Pass: array, size of the array.
*/
void freeStringArray(char **array, int size)
{
	int i;
	for (i = 0; i < size; i++)
	{	
		/* Makes sure only the used string pointers are freed. */
		if (array[i] != NULL)
		{
			free(array[i]);
		}
	}
	free(array);
}

/*
	Creates a 2D array of integers of the specified dimensions.

	Pass: number of rows, number of columns
	Return: the array.
*/
int** createArray(int rows, int columns)
{
	int i;
	int **array = malloc(sizeof(int*) * rows);
	for (i = 0; i < rows; i++)
	{
		array[i] = malloc(sizeof(int) * columns);
	}
	return array;
}

/*
	Frees up the space used by an 2D array of integers.

	Pass: the array, the number of rows in the array.
*/
void freeArray(int **array, int rows)
{
	int i;
	for (i = 0; i < rows; i++)
	{
		/* Free the second dimensions of the array before freeing the array. */
		free(array[i]);
	}
	free(array);
}

/*
	The test case for the above library of functions.
*/
int main(int argc, char *argv[])
{
	int *array;
	int width, height;
	int value;
	char **stringPtr;
	int **matrix;

		/* testing for part 1 */
	printf("Testing Part 1\n");
	width = 5;
	height = 6;

	array = create2DArray(height, width);

	printf("Store value 7 at [3,4].\n");
	set2DElement(array, 3, 4, 7);

	value = get2DElement(array, 3, 4);
	printf("Retrieve value %d from [3,4]\n\n", value);

	free2DArray(array);


		/* testing for part 2 */
	printf("Testing Part 2\n");
	stringPtr = createStringArray(100);

	printf("Store string - fred\n");
	setStringArray(stringPtr, 44, "fred");
	printf("Store string - barney\n");
	setStringArray(stringPtr, 80, "barney");

	printf("Get string - %s\n", getStringArray(stringPtr, 44));
	printf("Get string - %s\n", getStringArray(stringPtr, 80));
		/* test with NULL string */
	printf("Get string - %s\n\n", getStringArray(stringPtr, 3));

	freeStringArray(stringPtr, 100);


		/* testing for part 3 */
	printf("Testing Part 3\n");
	matrix = createArray(100, 100);

	printf("Store 33 44 55\n");
	matrix[22][76] = 33;
	matrix[83][29] = 44;
	matrix[99][65] = 55;

	printf("Retrieve %d %d %d\n", matrix[22][76], matrix[83][29],
	   matrix[99][65]);

	freeArray(matrix, 100);
	
	return(1);
}
