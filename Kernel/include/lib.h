#ifndef LIB_H
#define LIB_H

#include <stdint.h>

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);
void * kalloc(uint64_t,uint32_t);
void * alloc(int);
uint64_t alloc_process_stack(void* last, void*addr);
uint64_t free_process_stack(void* last, int cant);

void kfree(void*);
uint16_t getFlags(void);
int cmpstr(char * s1, char * s2);
char * cpuVendor(char *);


void
panic(char* msg);
typedef int bool;
enum { false, true };

#endif
