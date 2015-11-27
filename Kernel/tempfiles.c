#include <tempfiles.h>
//sacar
#include <naiveConsole.h>

static SemaphoreList * semaphore_list = NULL;
static MsgQueueList * msgQueue_list = NULL;

void printSemaphores();

void addSemaphore(Semaphore * sem){
	SemaphoreList * aux = kalloc(sizeof(semaphore_list), 0);
	aux->semaphore = sem;
	aux->next = semaphore_list;

	semaphore_list = aux;

	//printSemaphores();
}

void printSemaphores(){
	SemaphoreList * aux = semaphore_list;
	ncPrint("---Semaphore Print---\n");
	while(aux!=NULL){
		ncPrint("Semaforo: ");
		ncPrint(aux->semaphore->name);
		ncNewline();
		aux = aux->next;
	}
	ncPrint("------------\n");
}

Semaphore * getSemaphore(char * name){
	SemaphoreList * aux = semaphore_list;
	while(aux != NULL){
		if(cmpstr(aux->semaphore->name, name)){
			return aux->semaphore;
		}
		aux = aux->next;
	}
	return NULL;
}

void removeSemaphore(Semaphore * sem){
	SemaphoreList * aux = semaphore_list;
	SemaphoreList * prev = 	NULL;
	while(aux != NULL){
		if(aux->semaphore == sem){
			if(prev == NULL){
				semaphore_list = semaphore_list->next;
			}else{
				prev->next = aux->next;
			}
			free(aux);
			return;
		}else{
			prev = aux;
			aux = aux->next;
		}
		
	}
	
}

void addMessageQueue(MsgQueue * mq){
	MsgQueueList * aux = kalloc(sizeof(msgQueue_list), 0);
	aux->mq = mq;
	aux->next = msgQueue_list;

	msgQueue_list = aux;
}

MsgQueue * getMessageQueue(char * name){
	MsgQueueList * aux = msgQueue_list;
	while(aux != NULL){
		if(cmpstr(aux->mq->name, name)){
			return aux->mq;
		}
		aux = aux->next;
	}
	return NULL;
}

void removeMessageQueue(MsgQueue * mq){
	MsgQueueList * aux = msgQueue_list;
	MsgQueueList * prev = 	NULL;
	while(aux != NULL){
		if(aux->mq == mq){
			if(prev == NULL){
				msgQueue_list = msgQueue_list->next;
			}else{
				prev->next = aux->next;
			}
			free(aux);
			return;
		}else{
			prev = aux;
			aux = aux->next;
		}
	}
	
}