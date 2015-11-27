#define TOTAL_MEMORY 0x100000000
#define PAGE 0x1000
#define CANT_PAGES TOTAL_MEMORY/PAGE
#define CMP_START 0x8000000000000000

#include <pAllocator.h>
#include<lib.h>

static uint64_t* pMemory_BitMap;

void init_pMemoryAllocator(uint64_t total_memory)
{
	ncClear();
	pMemory_BitMap = (uint64_t*)  0x600000;
	int i = 0;
	int used = 0x800000/PAGE;
	ncPrintHex(used);
	ncNewline();
	while(i < used) {
		pMemory_BitMap[i] = 0x0;
		i++;
	}
	ncNewline();
	ncPrintHex(i);


	for(i; i < CANT_PAGES ; i++) {
		pMemory_BitMap[i] = 0xFFFFFFFFFFFFFFFF;
	}
	ncNewline();
	ncPrintHex(&pMemory_BitMap[i]);
	
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


