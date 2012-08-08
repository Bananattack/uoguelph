# Andrew Crowell
# 0545826
all: parselist

parselist: scanner.o parser.c parser.h
	gcc parser.c -o parselist -Wall -std=c99 -O3 scanner.o

scanner.o: scanner.c scanner.h
	gcc scanner.c -c -o scanner.o -Wall -std=c99 -O3

clean:
	rm parselist *.o
