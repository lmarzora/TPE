#include <stdint.h>
#include <process.h>
#include "../semaphore.h"

typedef struct
{
	int (*func) (int argc, char *argv);
	uint64_t argc;
	void* argv;
	char* name;
	int isForeground;
}process_data;


void yield(void);
uint64_t newProcess(process_data*);
void exit(int);
void sleep(uint64_t);
uint64_t schedule(uint64_t);

void killProcess(int num);

void block(ProcessQueue *queue, unsigned msecs);
Process * signal(ProcessQueue *queue);
void flushQueue(ProcessQueue *queue);

int isForeground(void);
void becomeForeground(void);
void freeTerminated();