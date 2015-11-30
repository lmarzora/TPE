#include<stdint.h>
#include<lib.h>
#include <stdint.h>
#include <process.h>

void printProcesses();


enum STATE {READY, BLOCKED, RUNNING, TERMINATED};

#define MAX_TICK 3;
#define FOREVER -1;




typedef int (*process_func) (int argc, char *argv);

struct stack_frame {

	uint64_t gs;
	uint64_t fs;
	uint64_t r15;
	uint64_t r14;
	uint64_t r13;
	uint64_t r12;
	uint64_t r11;
	uint64_t r10;
	uint64_t r9;
	uint64_t r8;
	uint64_t rsi;
	uint64_t rdi;
	uint64_t rbp;
	uint64_t rdx;
	uint64_t rcx;
	uint64_t rbx;
	uint64_t rax;

	
	//iretq hook
	uint64_t rip;
	uint64_t cs;
	uint64_t eflags;
	uint64_t rsp;
	uint64_t ss;
	uint64_t base;

	//exit hook


	
};



struct initial_stack
{
	struct stack_frame regs;
	process_func f;
	void * arg;
	int argc;
};

typedef struct stack_frame stack_frame;
typedef struct initial_stack initial_stack;



Process * create_process(process_func func, int argc, void *argv, char *name, int pid, int isForeground);
Process* get_last(ProcessQueue * pq);
void enqueue_q(Process *p, ProcessQueue * pq);
uint64_t select_process(uint64_t);
void dequeue_q(Process *p);
void dequeue_blocked(Process *p);
void ready(Process *p);
void enqueue_blocked(Process *p, int wakeup);
//static void block(ProcessQueue *queue, unsigned msecs);
//static Process * signal(ProcessQueue *queue);
//static void flushQueue(ProcessQueue *queue);
void process_list_add(Process *p);
void process_list_remove(Process *p);
void check_blocked_processes();
Process* peek_q(ProcessQueue *pq);
void print_all(char * texto);
void end_process(void);
uint64_t set_stack_frame(uint64_t*, process_func, uint64_t argc, void * argv);
void terminateProcess();
void yield_cpu();
Process * getProcessList();
int numProcesses();
int start(process_func func, int argc, void *argv);

int is_foreground(void);
void become_foreground(void);
int setScheduler();
int receives_data();

Process * findProcess(int num);
void delete_process(Process *p);


uint64_t get_process_SS();
uint64_t get_reserved_pages();
void add_cant_pages(uint64_t);
uint64_t get_pid();

void free_terminated();

