#include"memory.h"
#include<lib.h>
#include"../kSetUp.h"
#include<pAllocator.h>


#define PAGE_FRAME_MASK 0xFFFFFFFFFFFFF000ULL 

int getPages(int size, void**buff)
{
	void* p = myalloc(size);
	
	alloc_process_heap(p,p+size);
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
	if(size > PAGE)
		panic("size > PAGE");
	
	p = alloc_page();
	
	memset(p,k,size);

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

void alloc_process_stack(void* last, void*addr)
{
	
	//get addr page frame
	uint64_t page_frame, last_frame, p; 
	
	page_frame = (uint64_t)addr & PAGE_FRAME_MASK;
	last_frame = (uint64_t)last & PAGE_FRAME_MASK;
	
	ncNewline();
	ncPrint("addr_frame: ");
	ncPrintHex(page_frame);
	ncPrint(" last_frame: ");
	ncPrintHex(last_frame);
	ncNewline();
	
	//alloc pages
	for(p=page_frame;p>=last_frame;p-=PAGE)
	{
		alloc_pMemory(p,PAGE,1);
	}

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

void free(void* p)
{
	myfree(p);

}
