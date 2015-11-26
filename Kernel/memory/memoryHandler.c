#include"memory.h"
#include<lib.h>
#include"../kSetUp.h"

int getPages(int size, void**buff)
{
	void* p = myalloc(size);
	
	*buff = p;
	return 1;
}

void* alloc(int size)
{
	if(size < PAGE)
		size = PAGE;
	
	void* p;
	p = myalloc(size);

	return p;

}


void* kalloc(uint64_t size,uint32_t k)
{
	if(size < PAGE)
		size = PAGE;
	
	void* p;
	p = myalloc(size);
	
	memset(p,k,size);

	return p;
}

void setUpPageFrameAllocator(uint64_t memory_size)
{
	mem_setup(memory_size);
	//set kernel pages as used
	//set userand pages as used

}

void free(void* p)
{
	myfree(p);

}
