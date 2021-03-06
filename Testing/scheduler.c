#include<memory.h>


enum STATE {READY, BLOCKED, RUNNING, TERMINATED};

#define MAX_TICK 3;
#define FOREVER -1;
static volatile int curr_tick;
static volatile uint64_t total_ticks = 0;

typedef enum { false, true } boolean;

typedef int (*process_func) (int argc, char *argv);

struct ProcessQueue
{
	struct Process * head;
	struct Process * tail;
};

struct Process{
	struct Process *next;
	struct Process *prev;
	struct Process *blocked_prev;
	struct Process *blocked_next;
	struct Process *list_prev;
	struct Process *list_next;
	struct ProcessQueue *queue;
	char * name;
	int state;	
	int id;
	uint64_t wakeup;
	boolean atomic;
	boolean waiting;
};

typedef struct ProcessQueue ProcessQueue;
typedef struct Process Process;


static volatile Process *curr_process;
static ProcessQueue *pq_ready;
static ProcessQueue *pq_blocked;
static ProcessQueue *pq_terminated;
static Process *process_list;		
static unsigned num_processes;	


Process* get_last(ProcessQueue * pq);
void enqueue_q(volatile Process *p, ProcessQueue * pq);
void select_process();
static void dequeue_q(Process *p);
static void dequeue_blocked(Process *p);
static void ready(Process *p);
static void enqueue_blocked(Process *p, int wakeup);
static void block(ProcessQueue *queue, unsigned msecs);
static void process_list_add(Process *p);
static void process_list_remove(Process *p);
static void check_blocked_processes();
Process* peek_q(ProcessQueue *pq);
void print_all(char * texto);
void yield(void);
Process * create_process(process_func func, int argc, void *arg, const char *name, int pid);
void end_process(void);



int main(){

	pq_ready = calloc(1, sizeof(ProcessQueue));
	pq_blocked = calloc(1, sizeof(ProcessQueue));
	pq_terminated = calloc(1, sizeof(ProcessQueue));
	//pq_ready.head = NULL;
	//pq_ready.tail = NULL;

	curr_tick = MAX_TICK;


	int i;
	char buff[30];
	for(i=0; i<10; i++){
		Process *choto;
		if(i==0){
			choto = calloc(1, sizeof(Process));
			choto->atomic = false;
			choto->id = i;
			choto->state = RUNNING;
			curr_process = choto;
			choto->name = "0";
			process_list_add(choto);
		}else{
			s//printf(buff, "%d", i);
			choto = create_process(NULL, 0, NULL, buff, i);
			ready(choto);
		}
	}

	
	while((i = getchar()) != EOF){
		total_ticks++;
		print_all("Antes");
		check_blocked_processes();
		if((total_ticks % 8) == 0){
			//printf("%ld) Bloqueando a: %d\n", total_ticks, curr_process->id);
			block(NULL, 10);
		}
		else if((total_ticks % 5) == 0){
			delete_process(curr_process);
		}
		/*
		if(total_ticks == 12){
			//printf("%ld) Bloqueando a: %d\n", total_ticks, curr_process->id);
			block(NULL, total_ticks + 20);
		}
		if(total_ticks == 22){
			//printf("%ld) Bloqueando a: %d\n", total_ticks, curr_process->id);
			block(NULL, total_ticks + 10);
		}*/
		select_process();
		print_all("Despues");
		
	}



}

static void check_blocked_processes(){
	Process *p = peek_q(pq_blocked);
	while(p != NULL && p->wakeup <= total_ticks){
		ready(p);
		p = peek_q(pq_blocked);	
	}
}

void print_all(char * texto){

	/*
	int i = 0;
	Process * aux = process_list;
	//printf("Procesos: \n");
	for(i=0; i<num_processes; i++){
		//printf("Id: %d | ", aux->id);
		aux = aux->list_next;
	}
	//printf("\n");*/

	




	//printf("-------------------------\n");
	//printf("\n");
	int i = 0;
	Process * aux = process_list;
	//printf("Procesos: \n");
	for(i=0; i<num_processes; i++){
		//printf("Id: %d | ", aux->id);
		aux = aux->list_next;
	}
	//printf("\n");
	
	//printf("-------------------------\n");
	//printf("*** %s ***\n", texto);
	//printf("Curr Process: %d\n", curr_process->id);
	//printf("Readys:\n");
	Process * p;
	Process * first = peek_q(pq_ready);
	aux = first;
	if(first != NULL){
		//printf(" id: %d  |", first->id);
		first = first->next;
		while(first != aux){
			//printf(" id: %d  |", first->id);
			first = first->next;
		}
	}

	//printf("\nBlocked:\n");
	first = peek_q(pq_blocked);
	aux = first;
	if(first != NULL){
		//printf(" id: %d  |", first->id);
		first = first->blocked_next;
		while(first != NULL && first != aux){
			//printf(" id: %d  |", first->id);
			first = first->blocked_next;
		}
	}
	//printf("\n-------------------------\n");

	



	//printf("\n");
	//printf("-------------------------\n");
	//printf("\n");
	//printf("Terminated:\n");
	first = peek_q(pq_terminated);
	aux = first;
	if(first != NULL){
		//printf(" id: %d  |", first->id);
		first = first->next;
		while(first != aux){
			//printf(" id: %d  |", first->id);
			first = first->next;
		}
	}
	//printf("-------------------------\n");
	//printf("\n");
	//printf("\n");



}
void select_process(){

	if(curr_process->state == RUNNING){
		if(curr_process->atomic) return;
		curr_tick--;
		if(curr_tick > 0) return;
		curr_process->state = READY;
		enqueue_q(curr_process, pq_ready);
	}

	//printf("%ld) CHAU Process: %d\n", total_ticks, curr_process->id);

	Process *next_process = get_last(pq_ready);
	next_process->state = RUNNING;
	curr_tick = MAX_TICK;

	if(curr_process == next_process) return;

	curr_process = next_process;
	curr_process->next = NULL;
	curr_process->prev = NULL;
	curr_process->queue = NULL;
	//printf("%ld) HOLA Process: %d\n", total_ticks, curr_process->id);

	//HACER CAMBIO DE CONTEXTO


}

void enqueue_q(volatile Process *p, ProcessQueue *pq){
	if(pq == NULL){
		return;
	}
	if(pq->head == NULL){
		pq->head = p;
		pq->tail = p;
		p->next = p;
		p->prev = p;
		p->queue = pq;
		//printf("PRIMERO: %d\n", pq->head->id);
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

	if(pq->head == NULL){
		return NULL;
	}

	Process *aux = pq->head;
	pq->head->next->prev = pq->tail;
	pq->tail->next = pq->head->next;
	pq->head = pq->tail->next;
	aux->next = aux->prev = NULL;
	//aux->queue = NULL;
	return aux;
}

void atomic(){
	curr_process->atomic = true;
}


void unatomic(){
	curr_process->atomic = false;
}

static void 
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

static void 
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


static void
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




//antes deshabilitar interrupciones
//y despues habilitarlas
static void
ready(Process *p){
	//printf("%ld) Despertanto a: %d\n", total_ticks, p->id);

	dequeue_q(p);
	dequeue_blocked(p);
	enqueue_q(p, pq_ready);
	p->state = READY;
}

//antes deshabilitar interrupciones
//y despues habilitarlas --> Equivalente: WaitQueuedTime
static void block(ProcessQueue *queue, unsigned msecs){
	curr_process->state = BLOCKED;
	enqueue_q(curr_process, queue);
	if( msecs != -1){
		enqueue_blocked(curr_process, total_ticks + msecs);
	}
}

static void 
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

static void 
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

void yield(void)
{
	//desbilitar interrupciones
	enqueue_q(curr_process, pq_ready);
	curr_process->state = READY;
	//mt_reschedule();
	//select_process();
	//activar interrupciones
	
}

Process * create_process(process_func func, int argc, void *arg, const char *name, int pid){
	Process *p;

	// Alocar bloque de control
	/*
	task = Zalloc(sizeof(Task));
	SetName(task, name);
	if ( priority < MIN_PRIO )
		priority = MIN_PRIO;
	else if ( priority > MAX_PRIO )
		priority = MAX_PRIO;
	task->priority = priority;
	task->consnum = mt_curr_task->consnum;		// hereda número de consola
	task->tls = mt_curr_task->tls;				// hereda TLS*/

	// Alocar stack garantizando tamaño mínimo. Como éste es por lo
	// menos una página, va a ser alocado por páginas enteras.
	//task->stack = Malloc(stacksize < MIN_STACK ? MIN_STACK : stacksize);

	/* 
	Inicializar el stack simulando que wrapper(func, arg) fue interrumpida 
	antes de ejecutar su primera instrucción y empujó su contexto al stack. 
	Inicializamos eflags para que corra con interrupciones habilitadas y 
	apuntamos cs:eip a su primera instrucción. Son los registros que el i386
	empuja al stack cuando se produce una interrupción. Los demás registros 
	no importan, porque wrapper() todavía no comenzó a ejecutar.
	*/
	/*InitialStack *s = (InitialStack *)(task->stack + alloc_size(task->stack)) - 1;	

	s->regs.eip = (unsigned) wrapper;			// simular interrupción
	s->regs.cs = MT_CS;							// .
	s->regs.eflags = INTFL;						// .
	s->func = func;								// primer argumento de wrapper
	s->arg = arg;								// segundo argumento de wrapper

	task->esp = &s->regs;						// puntero a stack inicial
	*/


	p = calloc(1, sizeof(Process));
	p->atomic = false;
	p->id = pid;
	p->name = name;


	// Agregar a la lista de tareas
	atomic();
	process_list_add(p);
	unatomic();

	return p;
}

void delete_process(Process *p){
	if (p == curr_process){
		end_process();
		return;
	}


	p->atomic = false;
	ready(p);
}

void end_process(){
	curr_process->state = TERMINATED;
	process_list_remove(curr_process);
	enqueue_q(curr_process, pq_terminated);
}
