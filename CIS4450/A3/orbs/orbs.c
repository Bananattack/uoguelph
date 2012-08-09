// Andrew Crowell
// 0545826
// acrowell@uoguelph.ca
//
// rbs.c
//
// Parallelized code that implements the Red/Blue problem and calculates it in parallel.
#include "orbs.h"

typedef enum {
	BLANK,
	RED,
	BLUE
} Color;

char* board;

// N is the size of square board. 
// Determined at runtime by commandline input 
int N; 
// T is a number that must divide N. 
// Determined at runtime by commandline input. 
int T; 
// Number of times T divides N. 
int subregionCount; 
// The area in cells of the TxT rectangle (ie. T*T). 
int subregionCells; 
 
// W is the number of worker threads. 
// It is determined at runtime by commandline input 
int W; 
// row/column division calculated for each worker 
// ('leftover' handed to last worker). 
int rcSplit; 
int rcLeftOver;
 
// Number of full-steps over the board before termination. 
int maxFullsteps; 
// Number of half-steps over the board before termination. 
int maxHalfsteps;
 
// Max percent of red/blue within subrectangles of the board. 
// If this exceeds c%, then the program can exit. 
// If there is a difference in half-steps 
// between the current half-step and calculation half-step, 
// these numbers must be invalidated and reset to 0. 
int percentHalfsteps; 
int maxBluePercent; 
int maxRedPercent; 
int winPercent;

// Interactive mode is for debugging and timestepping the red/blue problem.
int INTERACTIVE;
// Used to skip the prompt until the counter hits 0
int interactiveHalfstepCounter;
int interactiveExit = 0;
char done = 0;

// Auto mode needs to print the arguments.
char* argsText;

// Helper function, don't call directly
double TimeHelper(int reset)
{
	static clock_t start = 0.0;
	if (reset)
	{
		start = clock();
		return 0.0;
	}
	return (double)(clock() - start)/CLOCKS_PER_SEC;
}

// StartTime resets timer
// EndTime returns no. of seconds of CPU time since StartTime
void StartTime()
{
	TimeHelper(1); // reset timer
}

double EndTime()
{
	return TimeHelper( 0 );
}

void PrintUsage(int invalid)
{
	if(invalid) puts("Stopping. Invalid arguments to program.\r\n");
	puts(
		"Usage:\r\n"
		"* pN : no. of processors (threads) to use\r\n"
		"* bN: width of board > 1, so board size is N x N cells\r\n"
		"* tN: width of one overlay tile, each N x N cells\r\n"
		"* cN: termination condition in integer percent, 1-100\r\n"
		"* mN: max. no. of full steps (additional terdmination condition)\r\n"
		"* sN: optional random seed\r\n"
		"* i: optional interactive mode switch\r\n"
	);
	SHUTDOWN(-1);
}

void PrintBoard(int halfsteps)
{
	int x, y;
#ifdef WIN_ANSI_ESCAPE
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	WORD attr;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &consoleInfo);
	attr = consoleInfo.wAttributes;
#endif

#ifdef WIN_ANSI_ESCAPE
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#else
	fputs("\x1B[40;37m", stdout);
#endif
	fputs("x", stdout);
	for(x = 0; x < N; x++)
	{
		printf("%d", x % 10);
	}
	for(y = 0; y < N; y++)
	{
#ifdef WIN_ANSI_ESCAPE
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#else
        fputs("\x1B[40;37m", stdout);
#endif
		printf("\r\n%d", y % 10);
		for(x = 0; x < N; x++)
		{
			switch(board[y * N + x])
			{
				case BLANK:
#ifdef WIN_ANSI_ESCAPE
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
#else
                    fputs("\x1B[30;47m", stdout);
#endif
					putc(' ', stdout); break;
				case RED:
#ifdef WIN_ANSI_ESCAPE
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_INTENSITY);
#else
                    fputs("\x1B[37;41m", stdout);
#endif
					putc('>', stdout); break;
				case BLUE: 
#ifdef WIN_ANSI_ESCAPE
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_INTENSITY);
#else
                    fputs("\x1B[37;44m", stdout);
#endif
					putc('v', stdout); break;
			}
		}
	}

	/* Back to white, print the step number. */
#ifdef WIN_ANSI_ESCAPE
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#else
    fputs("\x1B[40;37m", stdout);
#endif
	printf("\r\n\r\nSteps: %d.%d/%d.0\n", halfsteps / 2, halfsteps % 2 * 5, maxFullsteps);
	printf("Maximum Overlay Percentages (re-evaluated every full step.)\r\n");

	/* Print red percentage */
#ifdef WIN_ANSI_ESCAPE
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_INTENSITY);
#else
    fputs("\x1B[37;41m", stdout);
#endif
	printf("Red: %d%% ", maxRedPercent);

	/* Print blue percentage */
#ifdef WIN_ANSI_ESCAPE
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_INTENSITY);
#else
    fputs("\x1B[37;44m", stdout);
#endif
	printf("Blue: %d%%", maxBluePercent);

	/* A couple lines. */
#ifdef WIN_ANSI_ESCAPE
	/*SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);*/
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), attr);
#else
    fputs("\x1B[0m", stdout);
#endif
	printf("\r\n\r\n");
}

void PrintBoardToFile(int halfsteps, FILE* f)
{
	int x, y;
	double t;
	for(y = 0; y < N; y++)
	{
		for(x = 0; x < N; x++)
		{
			switch(board[y * N + x])
			{
				case BLANK:
					putc(' ', f); break;
				case RED:
					putc('>', f); break;
				case BLUE: 
					putc('v', f); break;
			}
		}
		fprintf(f, "\n");
	}

	t = EndTime();
	fprintf(f, "Arguments: %s| Steps: %d.%d/%d.0 | Max Red: %d%% | Max Blue: %d%% | Time: %lf seconds\n",
			argsText,
			halfsteps / 2, halfsteps % 2 * 5, maxFullsteps,
			maxRedPercent, maxBluePercent,
			t);
	fprintf(stdout, "Arguments: %s| Steps: %d.%d/%d.0 | Max Red: %d%% | Max Blue: %d%% | Time: %lf seconds\n",
			argsText,
			halfsteps / 2, halfsteps % 2 * 5, maxFullsteps,
			maxRedPercent, maxBluePercent,
			t);
}

int main(int argc, char** argv)
{
	FILE* f;
	int i, j;
    size_t size;
	int seedNumber;
	int REPORT = 0;
	int bytesToAllocateForArgsText = 0;

	puts("\r\n");
	puts(argv[0]);
	puts("\r\n");
	if(argc == 1)
	{
		PrintUsage(0);
	}

	// gather N, T, W, maxFullsteps, winPercent, seedNumber, INTERACTIVE from commandline args; 
	N = T = W = winPercent = INTERACTIVE = 0;
	maxFullsteps = -1;
	seedNumber = time(NULL);
	for(i = 1; i < argc; i++)
	{
		bytesToAllocateForArgsText += strlen(argv[i]) + 1;
		switch(argv[i][0])
		{
			case 'p':
				if(strlen(argv[i]) >= 2)
				{
					W = atoi(&(argv[i][1]));
				}
				else
				{
					printf("Invalid process count.\r\n");
					PrintUsage(1);
				}
				break;
			case 'b':
				if(strlen(argv[i]) >= 2)
				{
					N = atoi(&(argv[i][1]));
				}
				else
				{
					printf("Invalid size parameter for board.\r\n");
					PrintUsage(1);
				}
				break;
			case 't':
				if(strlen(argv[i]) >= 2)
				{
					T = atoi(&(argv[i][1]));
				}
				else
				{
					printf("Invalid size parameter for overlay.\r\n");
					PrintUsage(1);
				}
				break;
			case 'c':
				if(strlen(argv[i]) >= 2)
				{
					winPercent = atoi(&(argv[i][1]));
				}
				else
				{
					printf("Invalid win percentage.\r\n");
					PrintUsage(1);
				}
				break;
			case 'm':
				if(strlen(argv[i]) >= 2)
				{
					j = sscanf(&(argv[i][1]), "%d", &maxFullsteps);
					if(!j)
					{
						printf("Invalid max steps.\r\n");
						PrintUsage(1);
					}
				}
				else
				{
					printf("Invalid max steps.\r\n");
					PrintUsage(1);
				}
				break;
			case 's':
				if(strlen(argv[i]) >= 2)
				{
					j = sscanf(&(argv[i][1]), "%d", &seedNumber);
					if(!j)
					{
						printf("Invalid seed.\r\n");
						PrintUsage(1);
					}
				}
				else
				{
					PrintUsage(1);
				}
				break;
#ifndef DISABLE_INTERACTIVE
			case 'i':
				if(strlen(argv[i]) != 1)
				{
					printf("Invalid parameter %s.\r\n", argv[i]);
					PrintUsage(1);
				}
				INTERACTIVE = 1;
				break;
#endif
			case 'R':
				REPORT = 1;
				break;
			default:
				printf("Invalid parameter %s.\r\n", argv[i]);
				PrintUsage(1);
				break;
		}
	}

	argsText = malloc(bytesToAllocateForArgsText + 1);
	argsText[0] = 0;
	for(i = 1; i < argc; i++)
	{
		strcat(argsText, argv[i]);
		strcat(argsText, " ");
	}
	printf("Argument text: '%s' \r\n", argsText);

	srand(seedNumber);

	if(W <= 0)
	{
		printf("Invalid or nonexistant process count, must be greater than zero.\r\n");
		PrintUsage(1);
	}
	if(N < 2)
	{
		printf("Invalid or nonexistant size parameter for board, must be greater than or equal or 2.\r\n");
		PrintUsage(1);
	}
	if(T <= 0 || N % T != 0)
	{
		printf("Invalid or nonexistant size parameter for overlay.\r\n");
		PrintUsage(1);
	}
	if(winPercent <= 0)
	{
		printf("Invalid or nonexistant termination condition percent, must be (1-100).\r\n");
		PrintUsage(1);
	}
	if(maxFullsteps < 0)
	{
		printf("Invalid or nonexistant number of max full steps, must be greater than or equal to zero.\r\n");
		PrintUsage(1);
	}

	board = malloc(N * N);
	if(!board)
	{
		perror("Not enough memory.\r\n");
		SHUTDOWN(-1);
	}
	for(i = 0; i < N * N; i++)
	{
		board[i] = rand() % 3;
	}

	// Set up the miscellaneous. 
	maxHalfsteps = maxFullsteps * 2;
	percentHalfsteps = -1; 
	maxBluePercent = maxRedPercent = 0;

	omp_set_num_threads(W);

	StartTime();
	#pragma omp parallel
	{ 
		if(omp_get_thread_num() == 0)
		{
			W  = omp_get_num_threads();
			printf("W = %d\r\n", W);
			// If N is greater than or equal to the number of workers, there is work for every thread. 
			if(N >= W) 
			{ 
				// If workers divide N, then every worker does the same amount of work. 
				if(N % W == 0) 
				{ 
					rcLeftOver = rcSplit = N / W; 
				} 
				// If workers do not divide N, 
				// Then the truncation of N / W is handed to W - 1 of W workers, 
				// And the final worker will take all the remaining work. 
				else 
				{ 
					rcSplit = N / W; 
					rcLeftOver = N - (rcSplit * (W - 1)); 
				} 
			} 
			else 
			{ 
				// Each worker up to N takes 1 row. 
				// Leftover is set here, but won't actually used. 
				rcSplit = 1; 
				rcLeftOver = 1; 
			 
				// Set the number of workers equal to the size of the board, 
				// so that we don't spawn threads that will always be doing nothing. 
				W = N; 
			}
			// Calculate how many times T divides N in advance. 
			// This will be used a lot in the board evaluation phase. 
			subregionCount = N / T; 
			// Size of each subregion in cells. 
			subregionCells = T * T;
		}
		
		#pragma omp barrier
		printf("%d got here\r\n", omp_get_thread_num());
		
		ThreadExecute(omp_get_thread_num());
	}

	if(INTERACTIVE)
	{
		if(!interactiveExit)
		{
			PrintBoard(percentHalfsteps);
		}
	}
	else
	{
		f = fopen("redblue.txt", "w");
		PrintBoardToFile(percentHalfsteps, f);
		fclose(f);
	}

	free(argsText);
	free(board);

	SHUTDOWN(0);
	return 0;
}

void InteractivePrompt(int halfsteps)
{
	FILE* f;
	char buffer[256];
	int num;
	int done = 0;

	if(interactiveHalfstepCounter > 0)
	{
		interactiveHalfstepCounter--;
		if(interactiveHalfstepCounter != 0)
		{
			return;
		}
	}

	PrintBoard(halfsteps);

	while(!done)
	{
		fputs(">> ", stdout);
        fgets(buffer, 256, stdin);

		// A numeric value.
		if(sscanf(buffer, "%d", &num) > 0)
		{
			if(num <= 0)
			{
				fputs("\r\n\r\nError (full step count must be positive)\r\n\r\n", stdout);
			}
			else
			{
				interactiveHalfstepCounter = 2 * num - halfsteps % 2;
				done = 1;
			}
		}
		else
		{
			if(strlen(buffer) > 2)
			{
				buffer[0] = '~';
			}
			switch(buffer[0])
			{
				case '\r':
				case '\n':
					interactiveHalfstepCounter = 2 - halfsteps % 2;
					done = 1;
					break;
				case 'h':
					interactiveHalfstepCounter = 1;
					done = 1;
					break;
				case 'c':
					INTERACTIVE = 0;
					done = 1;
					break;
				case 'x':
					f = fopen("redblue.txt", "w");
					PrintBoardToFile(halfsteps, f);
					fclose(f);

					interactiveExit = 1;
					done = 1;
					break;
				default:
					fputs("\r\n\r\nError\r\n\r\n", stdout);
					break;
			}
		}
	}
}

void ThreadExecute(size_t thread)
{
	int localMaxRedPercent = 0; 
    int localMaxBluePercent = 0; 
	int i, x, y;
	int a;
	int lastBlocked;
	int halfsteps = 0;
    // Start row/column for this thread 
    // to iterate over (inclusive) 
    int rcStart; 
    // End row/column (exclusive) 
    int rcEnd; 
    
    rcStart = thread * rcSplit; 
	// Extraneous thread.
	if(thread > W - 1)
	{
		rcStart = rcEnd = -1;
	}
    // The last thread takes on the remaining work. 
    else if(thread == W-1)
    { 
        rcEnd = rcStart + rcLeftOver; 
    } 
    // The other threads do the same amount of work. 
    else 
    { 
        rcEnd = rcStart + rcSplit; 
    }

	#pragma omp critical
	{
		printf("Thread %d working on rows %d:%d\r\n", thread, rcStart, rcEnd);
	}

	#pragma omp barrier

	while(1)
	{
        // Board evaluation phase. 
        // Assign each thread to each rectangle that 
        // equals their thread id modulo W. 
        // eg. with 4x4 subregions of a 16x16 board and 3 threads, 
        // work assignment is like so: 
        //     1 2 3 1 
        //     2 3 1 2 
        //     3 1 2 3 
        //     1 2 3 1 

		localMaxRedPercent = localMaxBluePercent = 0;
        for(i = thread; i < subregionCount * subregionCount; i += W) 
        { 
            // Calculate the starting cell position of the particular 
            // TxT rectangle being examined. 
            int startX = (i % subregionCount) * T; 
            int startY = (i / subregionCount) * T;
			int redPercent = 0;
			int bluePercent = 0;
            
            // Tally counts for red and blue. 
            for(y = startY; y < startY + T; y++) 
            { 
                for(x = startX; x < startX + T; x++) 
                { 
					switch(board[y * N + x])
					{
						case RED: redPercent++; break;
						case BLUE: bluePercent++; break;
					}
                } 
            }

            redPercent = redPercent * 100 / subregionCells; 
            bluePercent = bluePercent * 100 / subregionCells; 
            localMaxRedPercent = (localMaxRedPercent < redPercent) ? redPercent : localMaxRedPercent; 
            localMaxBluePercent = (localMaxBluePercent < bluePercent) ? bluePercent : localMaxBluePercent;
		}

		// Mutual exclusion is needed to update 
		// global maximum percentages.
		#pragma omp critical
		{
			// If calculation was old (before last movements), 
			// set the current global maxima equal to 
			// this local maxima. 
			if(percentHalfsteps != halfsteps) 
			{ 
				maxRedPercent = localMaxRedPercent; 
				maxBluePercent = localMaxBluePercent; 
				percentHalfsteps = halfsteps; 
			} 
			// Set the current global maxima so that is equal to 
			//     max(current maximum, this thread's max)
			else 
			{ 
				maxRedPercent = (maxRedPercent < localMaxRedPercent) ? localMaxRedPercent : maxRedPercent; 
				maxBluePercent = (maxBluePercent < localMaxBluePercent) ? localMaxBluePercent : maxBluePercent; 
			}
		}

        
		// Inspections must complete before continuing. 
		#pragma omp barrier

		// Evaluate whether more iterations are needed. 
		// Termination will occur if any of these conditions is true 
		if(maxRedPercent > winPercent || maxBluePercent > winPercent || halfsteps >= maxHalfsteps)
		{
			break;
		}
		
#ifndef DISABLE_INTERACTIVE
		if(INTERACTIVE)
		{
			// Wait for input.
			if(thread == 0)
			{
				InteractivePrompt(halfsteps);
			}
			#pragma omp barrier
			if(interactiveExit)
			{
				break;
			}
		}
#endif

		// Red movement phase. 
        // Do this over every row we own.
        for(y = rcStart; y < rcEnd; y++)
        {
			a = y * N;
			lastBlocked = board[a] == RED;
			for(i = 0; i < N - 1; i++)
			{
				if(board[a + i] == RED && board[a + i + 1] == BLANK)
				{
					board[a + i] = BLANK;
					board[a + i + 1] = RED;
					// So we don't land on the red we just moved again.
					i++;
				} 
			}
			if(i == N - 1 && !lastBlocked && board[a + (N - 1)] == RED && board[a] == BLANK)
			{
				// The last piece needs to check if there was a red on the other side of the board before we started.\
				// If there was, don't move it.
				board[a + (N - 1)] = BLANK;
				board[a] = RED;
			}
		}

		// Red movement must complete before continuing. 
		#pragma omp barrier

		halfsteps++;

#ifndef DISABLE_INTERACTIVE
		if(INTERACTIVE)
		{
			// Wait for input.
			if(thread == 0)
			{
				InteractivePrompt(halfsteps);
			}
			#pragma omp barrier
			if(interactiveExit)
			{
				break;
			}
		}
#endif

		// Blue movement phase. 
        // Do this over every row we own.
        for(x = rcStart; x < rcEnd; x++)
        {

			lastBlocked = board[x] == BLUE;
			for(i = 0; i < N - 1; i++)
			{
				if(board[i * N + x] == BLUE && board[(i + 1) * N + x] == BLANK)
				{
					board[i * N + x] = BLANK;
					board[(i + 1) * N + x] = BLUE;
					// So we don't land on the red we just moved again.
					i++;
				}
			}
			if(i == N - 1 && !lastBlocked && board[(N - 1) * N + x] == BLUE && board[x] == BLANK)
			{
				// The last piece needs to check if there was a red on the other side of the board before we started.\
				// If there was, don't move it.
				board[(N - 1) * N + x] = BLANK;
				board[x] = BLUE;
			}
		}

		// Blue movement must complete before continuing. 
		#pragma omp barrier

		halfsteps++;
	}
}


