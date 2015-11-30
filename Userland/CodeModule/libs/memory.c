#include <memory.h>
#include <syscalls.h>
#include <lib.h>

#define PAGE 0x1000

void* malloc(int size)
{
	if(size < PAGE)
		size = PAGE;
	
	if(size == 0)
		return (void*)0;
	void* p ;
	alloc(&p,size);
	return p;

}

void* calloc(int size)
{
	void* p = malloc(size);
	memset(p,0,size);
	return p;

}

void free(void*p)
{
	free_mem(p);
}
