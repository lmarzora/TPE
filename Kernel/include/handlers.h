#ifndef HANDLERS_H
#define HANDLERS>H

#include <stdint.h>

void sti();
void irqDispatcher(int64_t);
void pit_handler();
void keyboard_handler();
void pic();

#endif
