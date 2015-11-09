#include<memory.h>
#include <syscalls.h>


void* malloc(int size)
{
	void**p;
	alloc(p,size);
	printLn("addr:");
	return *p;

}
