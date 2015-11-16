#include<memory.h>
#include<lib.h>

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


void* kalloc(int size,char k)
{
	if(size < PAGE)
		size = PAGE;
	
	void* p;
	p = myalloc(size);
	
	memset(p,k,size);

	return p;
}
