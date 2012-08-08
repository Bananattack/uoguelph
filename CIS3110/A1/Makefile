all: memtest

memtest: memtest.c memory.o
	gcc memtest.c -o memtest memory.o

memory.o: memory.c
	gcc memory.c -c
