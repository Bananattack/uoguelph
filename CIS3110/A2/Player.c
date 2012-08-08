/*
	Lotto Player Process
*/
#include <stdlib.h>
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
#define MAX_ROUNDS 5

int shmid = 31126;
int shmidg = 31126;
int semid = 31126;
key_t key;
int pid;
int wins, losses, bets = 0;

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
struct nextbet localbet;

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

void InitPlayer()
{
	int ret;

	key = ftok("/tmp", 31126);
	semid = semget(key, 5, IPC_EXCL | 0600);
	if(semid < 0)
	{
		perror("Could not access semaphore");
	}

	ret = shmget((key_t) key,
			sizeof(struct login)
				+ sizeof(struct bet)
				+ sizeof(struct nextbet),
			IPC_EXCL | 0600);
	if(ret < 0)
	{
		perror("Could not access shared memory");
	}
	if(semid < 0 || ret < 0)
	{
		exit(-1);
	}

	login =  (struct login*) shmat(ret, (char*) 0, 0);
	if(login < 0)
	{
		perror("Login not mapped");
		exit(-1);
	}

	winningbet = (struct bet*) ((char*) login + sizeof(struct login));
	nextbet = (struct nextbet*) ((char*) winningbet + sizeof(struct bet));

	ret = shmget((key_t) (key + 200), sizeof(global), IPC_EXCL | 0600);
	if(ret < 0)
	{
		perror("Could not access global");
		exit(-1);
	}
	global = (struct global*) shmat(ret, (char*) 0, 0);
	if(global < 0)
	{
		perror("Global not mapped");
		exit(-1);
	}
}

int CompareNumbers(const void* ls, const void* rs)
{
	int a = *((const short*) ls);
	int b = *((const short*) rs);
	return a - b;
}

void PickBet(short* number)
{
	int i, j;

	/* Populate the array */	
	for(i = 0; i < 6; i++)
	{
		number[i] = rand() % 49 + 1;
		/* Ensure each number is distinct */
		for (j = 0; j < i; j++)
		{
			/* Already used that number, try again. */
			if(number[j] == number[i])
			{
				i--;
				break;
			}
		}
	}

	/* Sort the array */
	qsort(number, 6, sizeof(short), CompareNumbers);
}


int BetOnce(int mult)
{
	int ret = 0;
	pid = getpid();
	PickBet(localbet.bet.number);
	localbet.pid = pid;
	localbet.bet.mult = mult;
	for(;;usleep(1000))
	{
		SemaphoreWait(semid, Winningbet);
		if(global->round != -1)
		{
			/* Avoid a deadlock/busy wait, sleep if there are no empty bet slots */
			if(SemaphoreInspect(semid, NextbetEmpty) < 1)
			{
				SemaphoreSignal(semid, Winningbet);
				continue;
			}
			SemaphoreWait(semid, NextbetEmpty);
			printf("Player %d bets: ", pid);

			*nextbet = localbet;
			localbet.round = nextbet->round = global->round;
			ShowBet(&(nextbet->bet));
			bets++;

			SemaphoreSignal(semid, NextbetFull);
		}
		else
		{
			printf("[%ld] asks: Lottery out of business?\n", pid);
			ret = 1;
		}
		SemaphoreSignal(semid, Winningbet);
		break;
	}

	/* Wait for next round to come */
	for(;;usleep(1000))
	{
		if(global->round == -1)
		{
			return ret;
		}
		if(global->round != localbet.round)
		{
			return 0;
		}
	}
}

int Login()
{
	struct login player_login;
	player_login.pid = getpid();
	for(;;usleep(1000))
	{
		/* Avoid a deadlock/busy wait, sleep if there are no empty login slots */
		if(!SemaphoreInspect(semid, LoginEmpty))
		{
			continue;
		}
		SemaphoreWait(semid, LoginEmpty);

		*login = player_login;
		printf("Submitted login for player %d\n", player_login.pid);

		SemaphoreSignal(semid, LoginFull);
		return;
	}
}

void CheckRoundOutcomes()
{
	SemaphoreWait(semid, Winningbet);
	/* Won the round! */
	if(winningbet->number[0] == getpid())
	{
		printf("Player %d won this round!\n", getpid());
		wins++;
	}
	else
	{
		losses++;
	}
	SemaphoreSignal(semid, Winningbet);

}

int main()
{
	int round;
	int sad = 0;
	union semun
	{
		int val;
		struct semid_ds* buf;
		unsigned short* array;
	} dummy;

	/*if(freopen("player.log", "w", stdout) == NULL)
	{
		perror("lotto cannot open log");
		exit(-1);
	}*/

	InitPlayer();
	Login();
	for(round = 0; round < MAX_ROUNDS; round++)
	{
		if(BetOnce((round + 2) * 4))
		{
			sad = 1;
			break;
		}
		CheckRoundOutcomes();
	}
	if(sad)
	{
		printf("Player %d exiting sadly. ;_;\n", getpid());
		fflush(stdout);
	}
	else
	{
		printf("Player %d exiting happily! ^_^\n", getpid());
		fflush(stdout);
	}
	printf("Player %d won %d times, and lost %d times of the %d times betting.\n", getpid(), wins, losses, bets);
	fflush(stdout);
	SemaphoreCallSummary(stdout);

	return 0;
}
