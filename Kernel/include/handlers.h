#ifndef HANDLERS_H
#define HANDLERS_H

#include <stdint.h>

void sti();
void cli();
void irqDispatcher(int64_t);
void pit_handler();
void keyboard_handler();
void pic();
void int80handler();
void int81handler();
int syscall(int,char*,int);
int setInterrupt(int valor);
void processHandler(char* nombre, void* func, int argc, void * argv);

#endif
