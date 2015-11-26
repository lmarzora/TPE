#include <pAllocator.h>
#include<lib.h>

uint64_t pMemory_BitMap[CANT_PAGES];

void init_pMemoryAllocator(uint64_t total_memory)
{
	int i;
	for(i = 0 ; i < 64 ; i++) {
		pMemory_BitMap[i] = 0x0;
	}
	for(i; i < CANT_PAGES ; i++) {
		pMemory_BitMap[i] = 0xFFFFFFFFFFFFFFFF;
	}
}


uint64_t* alloc_page()
{
	uint64_t i = 0, j = 0;

	while(i<CANT_PAGES && !pMemory_BitMap[i])
	{
		i++;
	}	
	if(i==CANT_PAGES)
	{
		panic("out of memory");
	}

	uint64_t cmp = CMP_START;
	while(!(pMemory_BitMap[i] & cmp))
	{
		j++;
		cmp >>= 1;
	}

	pMemory_BitMap[i] = pMemory_BitMap[i] ^ cmp;

	return (uint64_t*) (i*64*PAGE + j*PAGE) ;	
}

void free_page(uint64_t pAddress)
{
	int i, j;

	i = pAddress / (64*PAGE);
	j = pAddress % (64);

	uint64_t cmp = CMP_START >> j;
	pMemory_BitMap[i] = pMemory_BitMap[i] ^ cmp;

	return;
}



