//#include <stdint.h>
#include <string.h>
#include <lib.h>
#include "scheduler.h"
#include "../msgqueue.h"
#include <scheduler_interface.h>
#include <naiveConsole.h>


static volatile int curr_tick;
static volatile uint64_t total_ticks = 0;
static volatile uint64_t pids = 0;

static volatile Process *curr_process;
static ProcessQueue *pq_ready;
static ProcessQueue *pq_blocked;
static ProcessQueue *pq_terminated;
static Process *process_list;		
static unsigned num_processes;
static stack_frame init_stack;

SemaphoreList * semaphore_list;
MsgQueueList * msgQueue_list;


static volatile Process *foreground_process;

int jaja = 0;

void addSemaphore(Semaphore * sem){
	SemaphoreList * aux = kalloc(sizeof(semaphore_list), 0);
	aux->semaphore = sem;
	aux->next = semaphore_list;

	semaphore_list = aux;
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
		}
		aux = aux->next;
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
		}
		aux = aux->next;
	}
	
}

void printStack(uint64_t* rsp);

void nullProcess(void);

void nullProcess(void){
	while(1);
}

int setScheduler(){

	pq_ready = kalloc(sizeof(ProcessQueue), 0);
	pq_blocked = kalloc(sizeof(ProcessQueue), 0);
	pq_terminated = kalloc(sizeof(ProcessQueue), 0);
	
	curr_tick = MAX_TICK;

	curr_process=0;

}



void printPqReady(){

	ncPrint("----------\n");
	ncPrint("PQ HEAD: ");
	ncPrint(pq_ready->head->name);
	ncPrint("   |   PQ TAIL: ");
	ncPrint(pq_ready->tail->name);
	ncNewline();
	ncPrint("----------\n");

	ncPrint("********************\n");
	ncPrint("Current: ");
	ncPrint(curr_process->name);
	ncNewline();
	Process * aux;
	Process * first = peek_q(pq_ready);
	aux = first;
	if(first != NULL){
		ncPrint(first->name);
		ncPrint("  |  ");
		first = first->next;
		while(first != aux){
			ncPrint(first->name);
			ncPrint("  |  ");
			first = first->next;
		}
	}
	ncPrint("\n********************\n");

}

Process * getProcessList(){
	return process_list;
}

int numProcesses(){
	return num_processes;
}

void check_blocked_processes(){
	Process *p = peek_q(pq_blocked);
	while(p != NULL && p->wakeup <= total_ticks){
		ready(p);
		p = peek_q(pq_blocked);	
	}
}


uint64_t select_process(uint64_t old_rsp){
	
	if(curr_process){
		curr_process->rsp=old_rsp;

		if(curr_process->state == RUNNING){

			if(curr_process->die){
				end_process();
			}

			if(curr_process->atomic) return curr_process->rsp;
			curr_tick--;
			if(curr_tick > 0) return curr_process->rsp;
			curr_process->state = READY;
			enqueue_q(curr_process, pq_ready);
		}
	}
	/*ncPrint("Viejo: ");
	ncPrint(curr_process->name);
	ncNewline();*/

	curr_tick = MAX_TICK;
	Process *next_process = get_last(pq_ready);

	if(curr_process == NULL && next_process == NULL)
		return old_rsp;

	next_process->state = RUNNING;
	

	jaja++;
	if(curr_process == next_process) return curr_process->rsp;
	
	/*
	if(jaja<11){
		ncPrint("Viejo ");
		ncPrint(curr_process->name);
	}*/


	
	curr_process = next_process;
	/*if(jaja<11){


	
	ncPrint("\nNuevo ");
	ncPrint(curr_process->name);
	ncNewline();
	printPqReady();
	}*/
	
	curr_process->next = NULL;
	curr_process->prev = NULL;
	curr_process->queue = NULL;

	return curr_process->rsp;

}

void enqueue_q(volatile Process *p, ProcessQueue *pq){
	
	if(pq == NULL){
		pq = kalloc(sizeof(ProcessQueue), 0);
	}

	if(pq->head == NULL){
		pq->head = p;
		pq->tail = p;
		p->next = p;
		p->prev = p;
		p->queue = pq;

		return;
	}
	pq->head->prev = p;
	pq->tail->next = p;
	p->next = pq->head;
	p->prev = pq->tail;
	pq->tail = p;
	p->queue = pq;
	/*
	if(jaja<10){
		ncPrint("Encolo en medio: ");
		ncPrint(p->name);
		ncNewline();
	}*/

}

Process* peek_q(ProcessQueue *pq){
	return pq->head;
}

Process* get_last(ProcessQueue *pq){


	if(pq == NULL || pq->head == NULL){
		return NULL;
	}


	Process *aux = pq->head;
	if(aux->next == aux && aux->prev == aux){
		pq->head = NULL;
		pq->tail = NULL;
	}else{
		pq->head->next->prev = pq->tail;
		pq->tail->next = pq->head->next;
		pq->head = pq->tail->next;
	}
	
	aux->next = aux->prev = NULL;
	aux->queue = NULL;

	/*
	if(jaja < 10){
		ncPrint("Voy a sacar a ");
		ncPrint(aux->name);
		ncNewline();
	}*/
	
	return aux;
}

void atomic(){
	if(curr_process == NULL) return;
	curr_process->atomic = true;
}


void unatomic(){
	if(curr_process == NULL) return;
	curr_process->atomic = false;
}

void 
enqueue_blocked(Process *p, int wakeup)
{
	Process *ta;

	// Buscar donde insertar
	for ( ta = pq_blocked->head ; ta && wakeup > ta->wakeup ; ta = ta->blocked_next )
		;

	if ( ta )							// insertar antes de ta
	{
		if ( (p->blocked_prev = ta->blocked_prev) )
			ta->blocked_prev->blocked_next = p;
		else
			pq_blocked->head = p;
		ta->blocked_prev = p;
		p->blocked_next = ta;
	}
	else if ( (ta = pq_blocked->tail) )		// insertar al final de la cola
	{
		ta->blocked_next = pq_blocked->tail = p;
		p->blocked_prev = ta;
		p->blocked_next = NULL;
	}
	else								// la cola esta vacia
	{
		pq_blocked->head = pq_blocked->tail = p;
		p->blocked_next = p->blocked_prev = NULL;
	}
	p->wakeup = wakeup;
	p->waiting = true;
}

void 
dequeue_blocked(Process *p){
	if (p->waiting == false)
		return;
	if (p->blocked_prev)
		p->blocked_prev->blocked_next = p->blocked_next;
	else
		pq_blocked->head = p->blocked_next;
	if (p->blocked_next)
		p->blocked_next->blocked_prev = p->blocked_prev;
	else
		pq_blocked->tail = p->blocked_prev;
	p->blocked_next = p->blocked_prev = NULL;
	p->waiting = false;
}


void
dequeue_q(Process *p){
	ProcessQueue *pq = p->queue;
	if(pq == NULL)
		return;

	if (p->prev)
		p->prev->next = p->next;
	else
		pq->head = p->next;
	if ( p->next )
		p->next->prev = p->prev;
	else
		pq->tail = p->prev;
	p->next = p->prev = NULL;
	p->queue = NULL;

}


void
ready(Process *p){

	int valor = setInterrupt(0);

	dequeue_q(p);
	//dequeue_blocked(p);
	enqueue_q(p, pq_ready);
	p->state = READY;
	/*
	ncPrint("Poniendo ready ");
	ncPrint(p->name);
	ncNewline();*/

	setInterrupt(valor);
}

Process * signal(ProcessQueue *queue){
	Process *p;

	int valor = setInterrupt(0);
	if ( (p = get_last(queue)) )
	{

		//ncPrint(p->name);
		ready(p);
		
	}
	setInterrupt(valor);

	return p;
}

void flushQueue(ProcessQueue *queue)
{
	Process *p;

	if ( peek_q(queue) )
	{
		while ( (p = get_last(queue)) )
			ready(p);
		
	}
	
}


void block(ProcessQueue *queue, unsigned msecs){
	curr_process->state = BLOCKED;

	dequeue_q(curr_process);
	enqueue_q(curr_process, queue);
	if( msecs != -1){
		enqueue_blocked(curr_process, total_ticks + msecs);
	}

	call_pit();

}

void 
process_list_add(Process *p)
{
	num_processes++;

	if ( !process_list )
	{
		// Primera inserción (tarea nula)
		process_list = p;
		return;
	}

	// Insertar en la cabeza
	p->list_next = process_list;
	process_list->list_prev = p;
	process_list = p;
}

void 
process_list_remove(Process *p)
{
	num_processes--;

	if ( p == process_list )
	{
		// Primera tarea de la lista
		process_list = process_list->list_next;
		process_list->list_prev = NULL;
		return;
	}

	// Ni la primera ni la última
	p->list_prev->list_next = p->list_next;
	p->list_next->list_prev = p->list_prev;
}

void yield_cpu(void)
{
	int valor = setInterrupt(false);
	enqueue_q(curr_process, pq_ready);
	curr_process->state = READY;
	setInterrupt(valor);
	call_pit();

	
}

Process * create_process(process_func func, int argc, void *argv, const char *name, int pid, int isForeground){
	Process *p;


	void *rsp = kalloc(0x800000,0);
	rsp += 0x800000 - 1 - sizeof(stack_frame);

	uint64_t nRsp = set_stack_frame(rsp,func, argc, argv);

	p = kalloc(sizeof(Process), 0);
	p->atomic = false;
	p->die = false;
	p->id = pid;
	p->name = name;
	p->rsp = nRsp;
	p->queue = NULL;
	p->prev = NULL;
	p->next = NULL;

	if(isForeground){
		foreground_process = p;
	}

	// Agregar a la lista de tareas
	atomic();
	process_list_add(p);
	ready(p);
	

	unatomic();
	return p;
}

int createProcess(process_func func, int argc, void *argv, const char *name, int isForeground)
{
	uint64_t pid = pids++;

	Process* p = create_process(func,argc,argv,name,pid, isForeground);
	
	return pid;

}

uint64_t set_stack_frame(uint64_t *rsp,process_func func, uint64_t argc, void * argv){

	
	stack_frame *r = (stack_frame*) (rsp);
	
	
	r->gs = 0x001;
	r->fs = 0x002;
	r->r15= 0x003;
	r->r14= 0x004;
	r->r13= 0x005;
	r->r12= 0x006;
	r->r11= 0x007;
	r->r10= 0x008;
	r->r9= 0x009;
	r->r8= 0x00A;
	r->rsi= argc;
	r->rdi= func;
	r->rbp= 0x00D;
	r->rdx= argv;
	r->rcx= 0x00F;
	r->rbx= 0x010;
	r->rax= 0x011;
	
	//iretq hook
	
	r->rip= &start;
	r->cs= 0x008;
	r->eflags= 0x202;
	r->rsp= (uint64_t)&(r->base);
	r->ss= 0x000;



	
	r->base= 0x000;
	
	
	return &(r->gs);

}


Process * findProcess(int num){
	int i;
	Process * aux = process_list;
	for(i=0; i<num_processes; i++){
		if(aux->id == num){
			return aux;
		}
		aux = aux->list_next;
	}


	return NULL;
}
void delete_process(Process *p){
	
	if (p == curr_process){
		end_process();
		return;
	}

	p->die = true;
	p->atomic = false;
	
	ready(p);

}

void end_process(){

	if(curr_process == foreground_process){
		Semaphore * foreground_sem = getSemaphore("foreground_sem");
		SignalSem(foreground_sem);
	}

	curr_process->state = TERMINATED;
	process_list_remove(curr_process);
	enqueue_q(curr_process, pq_terminated);
	call_pit();
}


is_foreground(){
	return curr_process ==  foreground_process;
}

become_foreground(){
	foreground_process = curr_process;
}


void terminateProcess()
{
	free(curr_process->ss);
	end_process();	
	

}

int bedTime(int queueID,uint64_t time)
{
	ProcessQueue* queue = NULL;	

	block(queue,time);
	return 1;
}

void printStack(uint64_t* rsp)
{
	ncNewline();	
	int i;
	for(i=0;i<sizeof(stack_frame)/8 +2;i++)
	{	
		ncPrintHex(rsp[i]);
		ncPrint(" ");
	}


}

int start(process_func func, int argc, void *argv){
	
	(*func)(argc, argv);

	end_process();
	
}