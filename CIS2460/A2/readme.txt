========================
CIS*2430 Assignment 2
Andrew Crowell
0545826
acrowell@uoguelph.ca
========================

Summary
-------
To see my answer to each of these questions, please view
the included Summary.pdf document.

Question 4 Instructions
-----------------------
libsimrand.a is a library that implements a combined linear
congruental pseudorandom number generator. It follows the
specifications of the Assignment_2.pdf given for the assignment
as well as it can.

Unfortunately simRandPeriodicity()'s return value will not
fit into a value of a long, because the value returned
is already greater than 2^63-1.

To compile the library, use the included Makefile by
typing "make". This will generate libsimrand.a
and will also make a Monte Carlo simulation that uses
this library. You can run the simulation by typing "./Q4",
and piping the results into a .csv file if desired.

To link this library against another program, use
a command of the form:
	gcc YOURPROGRAM.c -L. -lsimrand -o YOURPROGRAM

The constants I used for libsimrand.a are included in the
header. The values are appropriate because the resultant
shape in Excel is relatively uniform.

For a histogram of the "Q4" simulation please see the
summary document mentioned above.


