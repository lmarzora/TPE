#ifndef HANDLERS_H
#define HANDLERS_H

#include <stdint.h>

void sti();
void cli();
void halt();
void call_pit();
void irqDispatcher(int64_t);
void pit_handler();
void keyboard_handler();
void pic();
void int80handler();
void int81handler();
void int82handler();
void int83handler();
void int84handler();
int syscall(int,char*,int);
void pageFaultHandler(void);
void pageFault(uint64_t,uint64_t);
int setInterrupt(int valor);
void doubleFaultHandler(void);
void beyondStack();
void moreStack();

void processHandler(char* nombre, void* func, int argc, void * argv, int isForeground);
void msgQueueHandler(int code, char * msg, char * nombre, unsigned max, unsigned size);
void semaphoreHandler(int code, char * nombre, int valor);
void cpuHandler(int code, int num);


#endif
