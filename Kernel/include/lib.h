#ifndef LIB_H
#define LIB_H

#include <stdint.h>

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);
void * kalloc(uint64_t,uint32_t);
void * alloc(int);
void free(void*);
uint64_t getFlags(void);


void
panic(char* msg);
typedef int bool;
enum { false, true };

#endif
