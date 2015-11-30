#ifndef TEMPFILES_H
#define TEMPFILES_H

#include <semaphore.h>
#include <msgqueue.h>
#include <string.h>

SemaphoreList * getSemaphoreList();
MsgQueueList * getMsgQueueList();

void addSemaphore(Semaphore * sem);
Semaphore * getSemaphore(char * name);
void removeSemaphore(Semaphore * sem);

void addMessageQueue(MsgQueue * mq);
MsgQueue * getMessageQueue(char * name);
void removeMessageQueue(MsgQueue * mq);

#endif
