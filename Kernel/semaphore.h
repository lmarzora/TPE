#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <process.h>

typedef struct Semaphore
{
	ProcessQueue	queue;
	char * 			name;
	unsigned		value;
} Semaphore;

typedef struct SemaphoreList
{
	Semaphore * semaphore;
	struct SemaphoreList * next;
} SemaphoreList;

Semaphore *CreateSem(char *name, unsigned value);
void DeleteSem(Semaphore *sem);
void WaitSem(Semaphore *sem);
void WaitSemTimed(Semaphore *sem, unsigned tics);
void SignalSem(Semaphore *sem);
unsigned ValueSem(Semaphore *sem);
void FlushSem(Semaphore *sem);

#endif