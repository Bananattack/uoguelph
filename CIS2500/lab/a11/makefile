all: part1 part2

part1: singlefile.c
	gcc singlefile.c -o part1 -ansi -Wall -pedantic

part2: libmatrix.a
	gcc main.c -o part2 -ansi -Wall -pedantic -L. -lmatrix

libmatrix.a: matrix.o
	ar cr libmatrix.a matrix.o

matrix.o: matrix.c
	gcc matrix.c -o matrix.o -c
