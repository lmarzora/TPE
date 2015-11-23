#ifndef PROCESS_H
#define PROCESS_H

#include <lib.h>

typedef enum { false, true } boolean;

struct ProcessQueue
{
	struct Process * head;
	struct Process * tail;
};

struct Process{
	uint64_t rsp;
	uint64_t ss;
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
	boolean die;
};

typedef struct ProcessQueue ProcessQueue;
typedef struct Process Process;

#endif 