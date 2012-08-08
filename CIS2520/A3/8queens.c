#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void PrintBoard(int** board, int n)
{
	int y, x;
	char c;
	printf("Solution:\n\t");
	for(y = 0; y < n; y++)
	{
		for(x = 0; x < n; x++)
		{
			printf("----");
		}
		printf("-\n\t");
		for(x = 0; x < n; x++)
		{
			c = ((y + x) % 2 == 0) ? ' ' : '#';
			printf("|%c%c%c", c, (board[y][x]) ? 'Q' : c, c);
		}
		printf("|\n\t");
	}
	for(x = 0; x < n; x++)
	{
		printf("----");
	}
	printf("-\n\t");
	printf("\n");
}

bool QueenPlacementIsSafe(int** board, int n, int queen_y, int queen_x)
{
	int y, x;

	/* Vertical up check */
	x = queen_x;
	for(y = queen_y - 1; y >= 0; y--)
	{
		if(board[y][x])
		{
			return false;
		}
	}

	/* Diagonal up-left check */
	y = queen_y - 1;
	x = queen_x - 1;
	while(y >= 0 && x >= 0)
	{
		if(board[y][x])
		{
			return false;
		}
		y--;
		x--;
	}

	/* Diagonal up-right check */
	y = queen_y - 1;
	x = queen_x + 1;
	while(y >= 0 && x < n)
	{
		if(board[y][x])
		{
			return false;
		}
		y--;
		x++;
	}

	return true;
}

bool _NQueens(int** board, int* solution_count, bool print_solutions, int n, int y)
{
	int x;
	for(x = 0; x < n; x++)
	{
		if(!QueenPlacementIsSafe(board, n, y, x))
		{
			continue;
		}
		board[y][x] = 1;
		if(y == n - 1)
		{
			*solution_count = *solution_count + 1;
			if (print_solutions)
			{
				PrintBoard(board, n);
			}
			board[y][x] = 0;
			return false;
		}
		else if(_NQueens(board, solution_count, print_solutions, n, y + 1))
		{
			return true;
		}
		board[y][x] = 0;
	}
	return false;
}

int NQueens(int n, bool print_solutions)
{
	int i, j;
	int solution_count = 0;
	int** board = malloc(sizeof(int*) * n);
	for(i = 0; i < n; i++)
	{
		board[i] = malloc(sizeof(int) * n);
		for(j = 0; j < n; j++)
		{
			board[i][j] = 0;
		}
	}
	_NQueens(board, &solution_count, print_solutions, n, 0);
	for(i = 0; i < n; i++)
	{
		free(board[i]);
	}
	free(board);
	return solution_count;
}

int main()
{
	int n = 0;
	bool done = false;
	bool print_solutions;
	char buffer[4096];
	int solution_count;

	while(n < 4)
	{
		printf("Please enter the size n of the chess board (n must be >= 4): ");
		fgets(buffer, 4096, stdin);
		sscanf(buffer, "%d", &n);
	}
	while(!done)
	{
		printf("Would you like to have solutions printed? (Y = yes, N = no): " );
		fgets(buffer, 4096, stdin);
		if(buffer[0] == 'y' || buffer[0] == 'Y')
		{
			done = true;
			print_solutions = true;
		}
		if(buffer[0] == 'n' || buffer[0] == 'N')
		{
			done = true;
			print_solutions = false;
		}
	}

	solution_count = NQueens(n, print_solutions);
	if(solution_count)
	{
		printf("Found %d distinct solutions for n=%d.\n", solution_count, n);
	}
	else
	{
		printf("No solutions for n=%d.\n", n);
	}
	return 0;
}
