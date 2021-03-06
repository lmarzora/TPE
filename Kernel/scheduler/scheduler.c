//#include <stdint.h>
#include <string.h>
#include <lib.h>
#include "scheduler.h"
#include <scheduler_interface.h>
#include <naiveConsole.h>
#include <handlers.h>
#include <tempfiles.h>


static volatile int curr_tick;
static volatile uint64_t total_ticks = 0;
static volatile uint64_t pids = 0;

//Volatile?
static Process *curr_process;
static ProcessQueue *pq_ready;
static ProcessQueue *pq_blocked;
static ProcessQueue *pq_terminated;
static ProcessQueue *pq_limbo;
static Process *process_list;		
static unsigned num_processes;
//static stack_frame init_stack;

static Process *foreground_process;

void printStack(uint64_t* rsp);

void nullProcess(void);

void nullProcess(void){
	while(1);
}

int setScheduler(){

	pq_ready = kalloc(sizeof(ProcessQueue), 0);
	pq_blocked = kalloc(sizeof(ProcessQueue), 0);
	pq_terminated = kalloc(sizeof(ProcessQueue), 0);

	pq_limbo = kalloc(sizeof(ProcessQueue), 0);
	

	curr_tick = MAX_TICK;

	curr_process=0;

	return 0;

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


	

	total_ticks++;
	check_blocked_processes();


	if(curr_process){
		curr_process->rsp=old_rsp;

		if(curr_process->state == RUNNING){

			//if(curr_process->die){
			//	end_process();
			//}

			if(curr_process->atomic) return curr_process->rsp;
			curr_tick--;
			if(curr_tick > 0) return curr_process->rsp;
			curr_process->state = READY;
			enqueue_q(curr_process, pq_ready);
		}

		
	}
	

	curr_tick = MAX_TICK;
	Process *next_process = get_last(pq_ready);


	if(curr_process == NULL && next_process == NULL)
		return old_rsp;

	next_process->state = RUNNING;
	
	if(curr_process == next_process) return curr_process->rsp;
	
	curr_process = next_process;

	curr_process->next = NULL;
	curr_process->prev = NULL;
	curr_process->queue = NULL;
	


	if(curr_process->die){
		end_process();
		return old_rsp;
	}




	return curr_process->rsp;

}

void enqueue_q(Process *p, ProcessQueue *pq){
	
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
	if(pq == NULL || p->waiting)
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
	dequeue_blocked(p);
	enqueue_q(p, pq_ready);
	p->state = READY;

	setInterrupt(valor);
}

Process * signal(ProcessQueue *queue){
	Process *p;

	int valor = setInterrupt(0);
	if ( (p = get_last(queue)) )
	{
		ready(p);
	}
	setInterrupt(valor);

	return p;
}

void flushQueue(ProcessQueue *queue)
{
	Process *p;
	int valor = setInterrupt(0);
	while ( (p = get_last(queue)) )
		ready(p);
		
	setInterrupt(valor);
	
}

void limbo(){
	block(pq_limbo, -1);
}

void block(ProcessQueue *queue, unsigned msecs){
	curr_process->state = BLOCKED;
	dequeue_q(curr_process);

	if(queue != NULL)
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

Process * create_process(process_func func, int argc, void *argv, char *name, int pid, int isForeground){
	Process *p;


		
	void *rsp = alloc(0x800000);
	
	void *ss = rsp;
	void* endStack = rsp;


	
	void * orig_rsp = rsp;

	rsp += 0x800000 - 1 - sizeof(stack_frame);
/*	
	ncNewline();
	ncPrint("endStack: ");
	ncPrintHex(endStack);
	ncNewline();
	ncPrint("setting up stack for process: ");
	ncPrint(name);
	ncNewline();
*/	
	uint64_t reserved_pages = alloc_process_stack(rsp - sizeof(stack_frame),rsp);
/*	
	ncPrintHex(get_pAddress(rsp - 0x1000));
	ncNewline();	
	ncPrint("ok\n");
*/
	uint64_t nRsp = set_stack_frame(rsp,func, argc, argv);

	p = kalloc(sizeof(Process), 0);
	p->atomic = false;
	p->die = false;
	p->id = pid;
	p->name = name;
	p->rsp = nRsp;

	p->ss = ss;

	p->stack = orig_rsp;

	p->queue = NULL;
	p->prev = NULL;
	p->next = NULL;
	p->reserved_pages = reserved_pages;

	if(isForeground){
		foreground_process = p;
		p->receives_data = true;
	}
	
	// Agregar a la lista de tareas
	atomic();
	process_list_add(p);
	ready(p);
	

	unatomic();
	return p;
}

int createProcess(process_func func, int argc, void *argv, char *name, int isForeground)
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
	if(p->state != READY)
		ready(p);
	

}

void end_process(){

	if(curr_process == foreground_process){
		Semaphore * foreground_sem = getSemaphore("foreground_sem");
		SignalSem(foreground_sem);
	}
	//ncPrint("Meto a ");
	//ncPrint(curr_process->name);
	//ncNewline();
	curr_process->state = TERMINATED;
	//process_list_remove(curr_process);
	enqueue_q(curr_process, pq_terminated);

	call_pit();
	
}


int is_foreground(){
	return curr_process ==  foreground_process;
}

int receives_data(){
	return curr_process->receives_data;
}

void become_foreground(){
	foreground_process = curr_process;
}

void free_terminated(){

	Process * p;
	
	while(true){
		atomic();
		p= get_last(pq_terminated);
		if(!p){
			unatomic();
			break;
		}
		process_list_remove(p);
		free_process_stack(p->ss,p->reserved_pages);
		kfree(p);
		
		unatomic();
	}
}


void terminateProcess()
{
	end_process();	
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

	return 0;
	
}



uint64_t get_process_SS(){
	return curr_process->ss;
}

uint64_t get_reserved_pages(){
	return curr_process->reserved_pages;
}

void add_cant_pages(uint64_t pages){
	curr_process->reserved_pages += pages;
}

uint64_t get_pid(){
	return curr_process->id;
}

