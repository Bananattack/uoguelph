// Andrew Crowell
// 0545826
// acrowell@uoguelph.ca
//
// rbs.h
//
// Header for the red/blue problem that sets up a few various defines and includes required libraries.
#ifndef RBS_H
#define RBS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <time.h>

#define MAXTHREADS 16		// for MacProc core i7

// for Windows
#ifdef WINDOWS
#	include <windows.h>
#	include <conio.h>
#	define WIN_ANSI_ESCAPE
#	define SHUTDOWN(arg) do { puts("Press any key..."); _getch(); exit(arg); } while(0)
/*#	define SHUTDOWN(arg) exit(arg)*/
#else
#	define SHUTDOWN(arg) exit(arg)
#endif

/*#define SPEEDUP_BY_IGNORABLE_RACE_COND*/
/*#define DISABLE_INTERACTIVE*/
void ThreadExecute(size_t arg);

#endif


