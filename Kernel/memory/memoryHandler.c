#include<memory.h>

int getPages(int size, void**buff)
{
	void* p = myalloc(size);
	memcpy(buff,&p,8);
	return 1;
}
