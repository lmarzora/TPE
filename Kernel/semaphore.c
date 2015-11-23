#include <handlers.h>
#include <scheduler_interface.h>
#include <lib.h>
#include "semaphore.h"

/*
CreateSem - aloca un semaforo y establece su cuenta inicial.
*/

Semaphore *CreateSem(char *name, unsigned value)
{
	Semaphore *sem = kalloc(sizeof(Semaphore), 0);

	sem->value = value;
	sem->name = name;
	addSemaphore(sem);
	return sem;
}

/*
DeleteSem - da de baja un semaforo.
*/

void DeleteSem(Semaphore *sem)
{
	int valor = setInterrupt(0);
	removeSemaphore(sem);
	flushQueue(&sem->queue);
	free(sem);
	setInterrupt(valor);
}

/*
WaitSem, WaitSemCond, WaitSemTimed - esperar en un semaforo.
WaitSem espera indefinidamente, WaitSemCond retorna inmediatamente y
WaitSemTimed espera con timeout. El valor de retorno indica si se consumio
un evento del semaforo.
*/

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

/*
SignalSem - senaliza un semaforo.
Despierta a la primera tarea de la cola o incrementa la cuenta si la cola
esta vacia.
*/

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

/*
ValueSem - informa la cuenta de un semaforo.
*/

unsigned ValueSem(Semaphore *sem)
{
	return sem->value;
}

/*
FlushSem - despierta todas las tareas que esperan en un semaforo.
Las tareas completan su WaitSem() con el status que se pasa como argumento.
Deja la cuenta en cero.
*/

void FlushSem(Semaphore *sem)
{
	int valor = setInterrupt(0);
	sem->value = 0;
	flushQueue(&sem->queue);
	setInterrupt(valor);
}
