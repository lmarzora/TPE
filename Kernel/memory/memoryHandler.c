#include<memory.h>

int getPages(int size, void**buff)
{
	void* p = myalloc(size);
	
	*buff = p;
	return 1;
}
