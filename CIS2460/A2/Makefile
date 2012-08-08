Q4: Q4.c libsimrand.a simRand.h
	gcc Q4.c -L. -lsimrand -o Q4
libsimrand.a: simRand.o
	ar cr libsimrand.a simRand.o
simRand.o: simRand.c simRand.h
	gcc simRand.c -c -o simRand.o
clean:
	@rm Q4 simRand.o libsimrand.a