
#include "msgqueue.h"



/*
CreateMsgQueue, DeleteMsgQueue - creacion y destruccion de colas de mensajes.
El parametro msg_max indica la maxima cantidad de mensajes a almacenar, y
msg_size el tamano de cada uno. 
*/

MsgQueue *
CreateMsgQueue(char *name, unsigned msg_max, unsigned msg_size)
{
	char nom_sem1[100];
	char nom_sem2[100];

	nom_sem1[0] = 'g';
	nom_sem2[0] = 'p';
	int i;
	for(i=1; name[i] != 0; i++){
		nom_sem1[i-1] = name[i];
		nom_sem2[i-1] = name[i];
	}

	MsgQueue *mq;
	unsigned size = msg_max * msg_size;

	//if ( size / msg_size != msg_max )	
	//	Panic("CreateMsgQueue %s: excede capacidad", name);

	mq = kalloc(sizeof(MsgQueue),0);
	mq->name = name;
	mq->msg_size = msg_size;
	mq->head = mq->tail = mq->buf = kalloc(size,0);
	mq->end = mq->buf + size;
	mq->sem_get = CreateSem(nom_sem1, 0);
	mq->sem_put = CreateSem(nom_sem2, msg_max);

	addMessageQueue(mq);

	return mq;
}

void
DeleteMsgQueue(MsgQueue *mq)
{
	DeleteSem(mq->sem_get);
	DeleteSem(mq->sem_put);
	free(mq->buf);
	free(mq->name);
	free(mq);
}

/*
GetMsgQueue, GetMsgQueueCond, GetMsgQueueTimed - lectura de un mensaje.
*/

void GetMsgQueue(MsgQueue *mq, void *msg)
{
	GetMsgQueueTimed(mq, msg, -1);
}


void GetMsgQueueTimed(MsgQueue *mq, void *msg, unsigned msecs)
{
	WaitSemTimed(mq->sem_get, msecs);

	int valor = setInterrupt(0);
	memcpy(msg, mq->head, mq->msg_size);
	mq->head += mq->msg_size;
	if ( mq->head == mq->end )
		mq->head = mq->buf;
	setInterrupt(valor);

	SignalSem(mq->sem_put);

}

/*
PutMsgQueue, PutMsgQueueCond, PutMsgQueueTimed - escritura de un mensaje.
*/

void
PutMsgQueue(MsgQueue *mq, void *msg)
{
	PutMsgQueueTimed(mq, msg, -1);
}



void
PutMsgQueueTimed(MsgQueue *mq, void *msg, unsigned msecs)
{
	WaitSemTimed(mq->sem_put, msecs);

	int valor = setInterrupt(0);
	memcpy(mq->tail, msg, mq->msg_size);
	mq->tail += mq->msg_size;
	if ( mq->tail == mq->end )
		mq->tail = mq->buf;
	setInterrupt(valor);

	SignalSem(mq->sem_get);
}

/*
AvailMsgQueue - indica la cantidad de mensajes almacenada en la cola.
*/

unsigned
AvailMsgQueue(MsgQueue *mq)
{
	return ValueSem(mq->sem_get);
}
