all: Lotto Player
Lotto: Lotto.c Semaphore.o
	gcc Lotto.c -o Lotto Semaphore.o
Player: Player.c Semaphore.o
	gcc Player.c -o Player Semaphore.o
Semaphore.o: Semaphore.c Semaphore.h
	gcc Semaphore.c -c -o Semaphore.o
