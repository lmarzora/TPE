#include<stdint.h>

typedef struct
{
	int (*func) (int argc, char *argv);
	uint64_t argc;
	void* argv;
	char* name;
}process_data;


void yield(void);
uint64_t newProcess(process_data*);
void exit(int);
int usleep(uint64_t);
uint64_t schedule(uint64_t);
