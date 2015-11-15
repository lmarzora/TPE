enum STATE {READY, BLOCKED, RUNNING, TERMINATED};

#define MAX_TICK 3;
#define FOREVER -1;

typedef enum { false, true } boolean;

typedef int (*process_func) (int argc, char *argv);

struct ProcessQueue
{
	struct Process * head;
	struct Process * tail;
};

struct stack_frame {
	//Registers restore context
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
};


typedef struct
{
	stack_frame regs;
	process_func f;
	void * arg;
	int argc;
}
initial_stack;


struct Process{
	uint64_t rsp;
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


Process* get_last(ProcessQueue * pq);
void enqueue_q(volatile Process *p, ProcessQueue * pq);
uint64_t select_process(uint64_t);
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
Process * create_process(process_func func, int argc, void *argv, const char *name, int pid);
void end_process(void);
void set_stack_frame(void*, void*, int, stack_frame*);

