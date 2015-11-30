#include"memory.h"
#include<lib.h>
#include"../kSetUp.h"
#include<pAllocator.h>


#define PAGE_FRAME_MASK 0xFFFFFFFFFFFFF000ULL 

int getPages(int size, void**buff)
{
	int valor = setInterrupt(0);
	void* p = myalloc(size);
	alloc_process_heap(p,p+size);
	*buff = p;
	setInterrupt(valor);
	return 1;
}


void free_user_heap(void*p)
{
	int valor = setInterrupt(0);
	myfree(p);


	free_pMemory(p);
	setInterrupt(valor);
}



void* alloc(int size)
{
	int valor = setInterrupt(0);

	if(size < PAGE)
		size = PAGE;
	
	void* p;
	p = myalloc(size);

	setInterrupt(valor);
	return p;

}


void* kalloc(uint64_t size,uint32_t k)
{
	int valor = setInterrupt(0);
	
	if(size < PAGE)
		size = PAGE;
	
	void* p;
	if(size > PAGE)
		panic("size > PAGE");
	
	p = alloc_page();
	
	memset(p,k,size);

	setInterrupt(valor);

	return p;
}



void alloc_process_heap(void*start, void*last)
{
	uint64_t page_frame,last_frame, p;
 
	page_frame = (uint64_t) start & PAGE_FRAME_MASK;
	last_frame = (uint64_t)last & PAGE_FRAME_MASK;
	
	for(p = page_frame; p <= last_frame; p+=PAGE)
	{
		alloc_pMemory(p,PAGE,1);
	}	


}

uint64_t alloc_process_stack(void* last, void*addr)
{

	int valor = setInterrupt(0);
	//get addr page frame
	uint64_t page_frame, last_frame, p; 
	
	page_frame = (uint64_t)addr & PAGE_FRAME_MASK;
	last_frame = (uint64_t)last & PAGE_FRAME_MASK;
/*	
	ncNewline();
	ncPrint("addr_frame: ");
	ncPrintHex(page_frame);
	ncPrint(" last_frame: ");
	ncPrintHex(last_frame);
	ncNewline();
*/	
	//alloc pages
	int i = 0;
	for(p=page_frame;p>=last_frame;p-=PAGE)
	{
		i++;
		alloc_pMemory(p,PAGE,1);
	}

	setInterrupt(valor);
	return i;

}


uint64_t free_process_stack(void* ss , int cant)
{
	int valor = setInterrupt(0);
	int i = cant;
	int64_t page_frame, last_frame, p;
	void* last = ss + 0x800000 - 1; 
	

	myfree(ss);	
	last_frame = (uint64_t) last & PAGE_FRAME_MASK;
	
	for(p=last_frame;i<=cant;p-=PAGE)
	{
		i++;
		free_pMemory(p);
	}

	setInterrupt(valor);
	return i;
	
}


void* setUserModule(void* pAddress)
{
	void* vAddress = myalloc(0x200000);

	mapUserModule(vAddress,pAddress);	

	return vAddress;
}

void setUpPageFrameAllocator(uint64_t memory_size)
{
	mem_setup(memory_size);

}


void kfree(void* p)
{
	int valor = setInterrupt(0);
	free_page(p);
	setInterrupt(valor);	
	
}

