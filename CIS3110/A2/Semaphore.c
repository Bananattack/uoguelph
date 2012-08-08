#include "Semaphore.h"


int wait_count, signal_count, inspect_count = 0;
void SemaphoreWait(int semaphore_set_id, int sem_num)
{
	struct sembuf sb;

	sb.sem_num = sem_num;
	sb.sem_op = -1;
	sb.sem_flg = 0;

	wait_count++;
	if(semop(semaphore_set_id, &sb, 1) == -1)
	{
		perror("SemaphoreWait() failed");
		exit(1);
	}
}

void SemaphoreSignal(int semaphore_set_id, int sem_num)
{
	struct sembuf sb;

	sb.sem_num = sem_num;
	sb.sem_op = 1;
	sb.sem_flg = 0;

	signal_count++;
	if(semop(semaphore_set_id, &sb, 1) == -1)
	{
		perror("SemaphoreSignal() failed");
		exit(1);
	}
}

int SemaphoreInspect(int semaphore_set_id, int sem_num)
{
	inspect_count++;
	return semctl(semaphore_set_id, sem_num, GETVAL);
}

void SemaphoreCallSummary(FILE* f)
{
	fprintf(f, "SemaphoreWait() called %d times\n", wait_count);
	fprintf(f, "SemaphoreSignal() called %d times\n", signal_count);
	fprintf(f, "SemaphoreInspect() called %d times\n", inspect_count);
	fflush(f);
}
