#include<memory.h>
#include <syscalls.h>
#include<debug.h>
#include <lib.h>

#define PAGE 0x1000

void* malloc(int size)
{
	if(size < PAGE)
		size = PAGE;
	void**p;
	alloc(p,size);
	return *p;

}

void* calloc(int size)
{
	void* p = malloc(size);
	memset(p,0,size);
	return p;

}
