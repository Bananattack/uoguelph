/*
	Lotto Server Process
*/
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#include "Semaphore.h"

#define LoginEmpty 0
#define LoginFull 1
#define Winningbet 2
#define NextbetEmpty 3
#define NextbetFull 4

#define MAX_PLAYERS 20
#define MAX_ROUNDS 20

union semun
{
	int val;
	struct semid_ds* buf;
	unsigned short* array;
};

int shmid = 31126;
int shmidg = 31126;
int semid = 31126;
key_t key;

struct login
{
	int pid;
	// more stuff;
};
struct login *login;

struct bet
{
	short mult;
	short number[6];
};
struct bet *winningbet;

struct global
{
	long round;
};
struct global *global;

struct nextbet
{
	int pid;
	int round;
	struct bet bet;
};
struct nextbet *nextbet;

struct play
{
	/* Process id */
	int pid;
	/* Number of bets placed */
	int bets;
	/* Number of lost rounds */
	int loss;
	/* Used to keep track of last round a bet was placed */
	int round;
	/* The placed bet for the round, use the 'round' member
		to see that it belongs to the current round */
	struct bet bet;
} player[MAX_PLAYERS];
int numplayers = 0;

/* If an operation returns a negative error code, terminate the program. */
void SVC(int code, char* string)
{
	if(code < 0)
	{
		perror(string);
		exit(-1);
	}
	return;
}

void ShowBet(struct bet* b)
{
	int i;
	if(b == NULL)
	{
		return;
	}
	printf("$%d on the numbers ", b->mult);
	for(i = 0; i < 6; i++)
	{
		printf("%d ", b->number[i]);
	}
	printf("\n");
}

void FinalReport()
{
	int i, j;
	struct bet dummy;
	for(i = 0; i < numplayers; i++)
	{
		dummy.mult = player[i].loss;
		dummy.number[0] = player[i].bets;
		for(j = 1; j < 6; j++)
		{
			dummy.number[j] = 0;
		}
	}
	global->round = -1;
}

void InitGlobal()
{
	int i;
	union semun arg;

	key = ftok("/tmp", 31126);
	semid = semget((key_t)key, 5, 0600 | IPC_CREAT);
	if(semid < 0)
	{
		perror("Could not create sem");
	}

	/* Initialize the semaphores */
	arg.val = 1;
	SVC(semctl(semid, Winningbet, SETVAL, arg), "Failed to initialize mutex Winningbet");

	arg.val = 0;
	SVC(semctl(semid, LoginFull, SETVAL, arg), "Failed to initialize semaphore LoginFull");
	SVC(semctl(semid, NextbetFull, SETVAL, arg), "Failed to initialize semaphore NextbetFull");

	/* Only one request can be handled at a time. */
	arg.val = 1;
	SVC(semctl(semid, LoginEmpty, SETVAL, arg), "Failed to initialize semaphore LoginEmpty");
	SVC(semctl(semid, NextbetEmpty, SETVAL, arg), "Failed to initialize semaphore NextbetEmpty");

	shmid = shmget((key_t) key,
			sizeof(struct login)
				+ sizeof(struct bet)
				+ sizeof(struct nextbet),
			IPC_CREAT | 0600);
	if(shmid < 0)
	{
		perror("Could not create shared mem");
	}
	if(semid < 0 || shmid < 0)
	{
		exit(-1);
	}

	login =  (struct login*) shmat(shmid, (char *) 0, 0);
	if(login == (struct login*)(-1))
	{
		perror("Login not mapped");
		exit(-2);
	}
	winningbet = (struct bet*) ((char*) login + sizeof(struct login));
	nextbet = (struct nextbet*) ((char*) winningbet + sizeof(struct bet));

	shmidg = shmget((key_t) (key + 200), sizeof(global), 0600 | IPC_CREAT);
	if(shmidg < 0)
	{
		perror("Could not access global");
	}

	global = (struct global*) shmat(shmidg, (char*) 0, 0);
	if(global == (struct global*)(-1))
	{
		perror("Global not mapped");
		exit(-2);
	}
}

void InitLotto()
{
	/* Lotto begins at a fictitious round -1. */
	global->round = -1;
	/* Clear the player array so everything is initialized */
	memset(player, 0, sizeof(player));
}

void OneRound()
{
	int found;
	int winner;
	int i;
	int player_index;

	/* This round lasts five seconds, check for updates every 100,000 microseconds */
	for(i = 0; i <= 50; usleep(100000), i++)
	{
		/* If LoginFull > 0, and there are free player slots, handle a login */
		if(SemaphoreInspect(semid, LoginFull) > 0 && numplayers < MAX_PLAYERS)
		{
			/* Decrement and wait for LoginFull() */
			SemaphoreWait(semid, LoginFull);

			/* Critical section, add player */
			player[numplayers].bets = 0;
			player[numplayers].loss = 0;
			player[numplayers].pid = login->pid;
			printf("Added player %d\n", login->pid);
			fflush(stdout);
			numplayers++;

			/* Increment and unlock LoginEmpty() */			
			SemaphoreSignal(semid, LoginEmpty);
			continue;
		}
		/* If NextbetFull > 0, handle a bet. */
		else if(SemaphoreInspect(semid, NextbetFull))
		{
			/* Decrement and wait for NextbetFull() */
			SemaphoreWait(semid, NextbetFull);

			/* Critical section, sort out a bet request */
			/* Only process this bet if it was for this round */
			if(nextbet->round == global->round)
			{
				for(player_index = 0; player_index < numplayers; player_index++)
				{
					/*
						Find the player by their pid, and store their nextbet
						If the player hasn't logged in, we don't see their bet.
						If the player already placed a bet this round, ignore a second wager.
					*/
					if(player[player_index].pid == nextbet->pid
							&& (player[player_index].round != global->round
								|| player[player_index].bets == 0
							)
						)
					{
						printf("Bet received from player %d: ", nextbet->pid);
						ShowBet(&(nextbet->bet));
						fflush(stdout);

						player[player_index].bets++;
						player[player_index].bet = nextbet->bet;
						player[player_index].round = nextbet->round;
						break;
					}
				}
			}

			/* Increment and unlock NextbetEmpty() */			
			SemaphoreSignal(semid, NextbetEmpty);
			continue;
		}
	}
	/* End of the round -- figure out the winning bet */
	SemaphoreWait(semid, Winningbet);
	/* Check that SOMEONE placed a bet this round, or else there was no winner
		- meaning everyone loses */
	found = 0;
	for(player_index = 0; player_index < numplayers; player_index++)
	{
		if(player[player_index].bets && player[player_index].round == global->round)
		{
			found = 1;
			break;
		}
	}
	/* If nobody won, we can skip randomization, there simply is no winner */
	if(!found)
	{	
		winner = -1;
	}
	/* Somebody won, randomize until we land on a player who placed a bet */
	else
	{
		do
		{
			winner = rand() % numplayers;
		} while(!player[winner].bets || player[winner].round != global->round);
	}
	if(winner != -1)
	{
		*winningbet = player[winner].bet;
	}
	else
	{
		printf("Nobody placed any bets this round\n");
		fflush(stdout);
	}
	/* Update every player's loss count */
	for(player_index = 0; player_index < numplayers; player_index++)
	{
		if(player_index != winner)
		{
			player[player_index].loss++;
			/* Add any losing bets to the winningbet.mult. */
			if(player[player_index].bets && player[player_index].round == global->round)
			{
				winningbet->mult += player[player_index].bet.mult;
			}
		}
	}
	for(i = 0; i < 6; i++)
	{
		winningbet->number[i] = 0;
	}
	if(winner != -1)
	{
		printf("Winning bet of round %d was by player %d (%d wins, %d losses): ",
			global->round, player[winner].pid, (player[winner].bets - player[winner].loss), player[winner].loss);
		ShowBet(winningbet);
		fflush(stdout);
		winningbet->number[0] = player[winner].pid;
	}

	SemaphoreSignal(semid, Winningbet);
}

int main()
{
	int round;
	union semun dummy;

	if(freopen("lotto.log", "w", stdout) == NULL)
	{
		perror("lotto cannot open log");
		exit(-1);
	}

	InitGlobal();
	InitLotto();
	
	for(round = 0; round < MAX_ROUNDS; round++)
	{
		printf("Round %d\n", round);
		global->round = round;
		OneRound();
	}
	FinalReport();

	/* Sleep to give players a chance to see it is over */
	sleep(5);

	SVC(shmctl(shmid, IPC_RMID, (struct shmid_ds*) 0), "Shm rm fails");
	SVC(shmctl(shmidg, IPC_RMID, (struct shmid_ds *) 0), "Shm rm fails");
	SVC(semctl(semid, 0, IPC_RMID, dummy), "Sem rm fails");
	SemaphoreCallSummary(stdout);

	return 0;
}

