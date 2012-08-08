========================
CIS*2460 Assignment 3
Andrew Crowell
0545826
acrowell@uoguelph.ca
========================

Summary
-------
To see my answer to each of these questions, please view
the included A3.pdf document.

Instructions on Compiling and Running the Programs
--------------------------------------------------
To remove existing binaries, use:
	make clean

To compile the code, use the following command:
	make

This will generate three programs: Q1, Q2, and Q3,
as well as an object file simRand.o

To compile with verbose debugging, please type:
	make debug

To invoke and run any of the question simulations, run
via the command line with the form:
	./Q<n> <number of days>

For example:
	./Q1 1000

The source code can be found in Question*.[ch] for the programs,
and simRand.[ch] for the random number generator library (Which
was supplied for last assignment).

This code was written through Visual Studio 2008 and then
later adjusted for gcc, and presumably works on multiple
platforms. The last tested platform however, was
Ubuntu 8.04 Hardy Heron, so a Linux based operating system
is preferred for testing this application.
