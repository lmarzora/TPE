#ifndef PROCESS_H
#define PROCESS_H

#include <lib.h>

struct ProcessQueue
{
	struct Process * head;
	struct Process * tail;
};

struct Process{
	uint64_t rsp;
	uint64_t ss;
	uint64_t reserved_pages;
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
	bool atomic;
	bool waiting;
	bool die;
};

typedef struct ProcessQueue ProcessQueue;
typedef struct Process Process;

#endif 
