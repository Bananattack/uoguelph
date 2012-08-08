#ifndef SEMAPH_H
#define SEMAPH_H

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

extern int wait_count, signal_count, inspect_count;
void SemaphoreWait(int semaphore_set_id, int sem_num);
void SemaphoreSignal(int semaphore_set_id, int sem_num);
int SemaphoreInspect(int semaphore_set_id, int sem_num);
void SemaphoreCallSummary(FILE* f);

#endif
