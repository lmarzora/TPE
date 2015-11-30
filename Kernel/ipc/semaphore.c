#include <handlers.h>
#include <scheduler_interface.h>
#include <tempfiles.h>
#include <lib.h>
#include <semaphore.h>

Semaphore *CreateSem(char *name, unsigned value)
{
	Semaphore *sem = kalloc(sizeof(Semaphore), 0);

	sem->value = value;
	sem->name = name;
	addSemaphore(sem);
	return sem;
}


void DeleteSem(Semaphore *sem)
{
	int valor = setInterrupt(0);
	removeSemaphore(sem);
	flushQueue(&sem->queue);
	kfree(sem);
	setInterrupt(valor);
}


void WaitSem(Semaphore *sem)
{
	WaitSemTimed(sem, -1);
}


void WaitSemTimed(Semaphore *sem, unsigned tics)
{
	int valor = setInterrupt(0);
	if ( sem->value > 0 )
	{
		sem->value--;
	}else{
		block(&sem->queue, tics);
	}
	setInterrupt(valor);
	
}


void SignalSem(Semaphore *sem)
{

	int valor = setInterrupt(0);
	//ncPrint("Signal\n");
	if ( !signal(&sem->queue) ){

		//ncPrint("Aumento semaforo\n");
		sem->value++;
	}
		
	setInterrupt(valor);
}


unsigned ValueSem(Semaphore *sem)
{
	return sem->value;
}



void FlushSem(Semaphore *sem)
{
	int valor = setInterrupt(0);
	sem->value = 0;
	flushQueue(&sem->queue);
	setInterrupt(valor);
}
