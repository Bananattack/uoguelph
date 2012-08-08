all: Q1 Q2 Q3
debug: Q1DEBUG Q2DEBUG Q3DEBUG
Q1: Question1.c simRand.o
	gcc Question1.c simRand.o -lm -o Q1
Q2: Question2.c simRand.o
	gcc Question2.c simRand.o -lm -o Q2
Q3: Question3.c Question3.h simRand.o
	gcc Question3.c simRand.o -lm -o Q3
Q1DEBUG: Question1.c simRand.o
	gcc -DDEBUG=1 Question1.c simRand.o -lm -o Q1
Q2DEBUG: Question2.c simRand.o
	gcc -DDEBUG=1 Question2.c simRand.o -lm -o Q2
Q3DEBUG: Question3.c Question3.h simRand.o
	gcc -DDEBUG=1 Question3.c simRand.o -lm -o Q3
simRand.o: simRand.c simRand.h
	gcc simRand.c -lm -c -o simRand.o

clean:
	rm -f simRand.o Q1 Q2 Q3
