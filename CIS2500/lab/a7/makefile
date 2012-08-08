all: main main2
	
main: main.o add.o
	gcc main.o add.o -o main

main.o: main.c
	gcc -c main.c -o main.o

add.o: add.c
	gcc -c add.c -o add.o

main2: main2.o f1.o f2.o
	gcc main2.o f1.o f2.o -o main2

main2.o: main2.c
	gcc -c main2.c -o main2.o

f1.o: f1.c
	gcc -c f1.c -o f1.o

f2.o: f2.c
	gcc -c f2.c -o f2.o

clean:
	rm *.o main main2
