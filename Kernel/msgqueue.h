#ifndef MSGQUEUE_H
#define MSGQUEUE_H
#include "semaphore.h"

typedef struct MsgQueue
{
	char *			name;
	Semaphore *		sem_get;
	Semaphore *		sem_put;
	unsigned		msg_size;
	char *			buf;
	char *			head;
	char *			tail;
	char *			end;
} MsgQueue;

typedef struct MsgQueueList
{
	MsgQueue *mq;
	struct MsgQueueList * next;
} MsgQueueList;

MsgQueue * CreateMsgQueue(char *name, unsigned msg_max, unsigned msg_size);

void DeleteMsgQueue(MsgQueue *mq);

void GetMsgQueue(MsgQueue *mq, void *msg);

void GetMsgQueueTimed(MsgQueue *mq, void *msg, unsigned msecs);

void PutMsgQueue(MsgQueue *mq, void *msg);

void PutMsgQueueTimed(MsgQueue *mq, void *msg, unsigned msecs);

unsigned AvailMsgQueue(MsgQueue *mq);


#endif