#include<memory.h>
#include<lib.h>
#include<naiveConsole.h>
#include<stdint.h>
#include<inttypes.h>


#define PAGE 0x1000
#define MAX_LEVEL 8
#define MAX_INDEX 7
#define TOTAL_MEMORY 0x100000000
#define CMP  0x8000000000000000
#define HEAP_START 0x700000

static uint64_t * bitmaps[MAX_LEVEL + 1];
extern uint8_t endOfKernel;


int block_size(int level)
{
	return PAGE * (1ull << level);

}
int level_size(int level)
{
	return TOTAL_MEMORY / block_size(level);

}


void
mem_setup(unsigned himem_size)
{
	extern char _end;
	ncClear();
	ncNewline();
	ncPrint("setting up page frame allocator");
	ncNewline();
	
	ncPrint("bitmap addr: ");
	ncPrintHex(&bitmaps);
	ncNewline();
	uint64_t addr = (void*) 0x600000;
	int i;
	for(i=0;i<MAX_LEVEL;i++)
	{	
		bitmaps[i] = addr;
		addr += (level_size(i)) * sizeof(uint64_t);
		
		ncPrint("level: ");
		ncPrintDec(i);;		
		ncNewline();
		/*
		ncPrint("level_addr: ");
		ncPrintHex(bitmaps[i]);
		ncPrint(" level_size: ");
		ncPrintDec(level_size(i));
		ncPrint(" block_size: ");
		ncPrintDec(block_size(i));
		ncNewline();
		ncClear();
		ncPrint("setting lvl");
		ncNewline();
		*/
		memset(bitmaps[i],0,level_size(i)*sizeof(uint64_t));
	
	}
	memset(bitmaps[MAX_INDEX],0xAAAAAAAA,level_size(i)*sizeof(uint64_t));
	
}















void * 
myalloc(uint64_t bytes)
{
	//ncPrint("MYALLOC\n");
	//ncPrint("bytes:%d\n",bytes);
	void * p;
	int level = getLevel(bytes);
	if (level == -1)
	{	
		panic("memory excedes limit");
	}
	//ncPrint("bytes: %d, level: %d\n",bytes,level); 
	int t = level;
	do
	{		
		p = getBlock(t);
		//ncPrint("address: %x\n",p);
		t++;
	}	
	while (t < MAX_LEVEL && p == (void*)0xDEAD);
	

	if (t > MAX_LEVEL || p == (void*)0xDEAD)
	{
		ncPrintHex(p);
		ncNewline();
		panic("Out of memory");
		return (void*) 0;
	}
	//ncPrint("\n\n\nreserving %d bytes from address %x\n",bytes,p);
	ncClear();
	reserveMemory(p,bytes,t-1);
	//ncPrint("addr: %"PRIx64"\n",p);	
	return p + HEAP_START;
}

int
reserveMemory(void* p, uint64_t bytes, int level)
{
	
	ncPrint("level: ");
	ncPrintDec(level);
	ncPrint(" , p: ");
	ncPrintHex(p);
	ncPrint(" , bytes; ");
	ncPrintDec(bytes);
	ncNewline();
	
	if(!bytes)
	{
		//ncPrint(" setting free ");
		set(level,p,1,0);
		return 0;
	}
	if(bytes > block_size(level))
	{
		//ncPrint(" setting part ");
		set(level,p,0,0);
		return bytes - block_size(level);
	}
	if(level && bytes < block_size(level))
	{
		bytes = reserveMemory(left(p,level),bytes,level-1);
		bytes = reserveMemory(right(p,level),bytes,level-1);
	}
	if(bytes)
		bytes = bytes - block_size(level);
	//ncPrint(" setting full ");
	set(level,p,0,1);
	return bytes;	

}

void*
left(void* p, int level)
{
	return p;
}

void*
right(void*p, int level)
{
	return p + block_size(level - 1);
}

int
getLevel(uint64_t bytes)
{
	//ncPrint("getting level\n");
	ncNewline();
	double l = (bytes/PAGE) ;
		if(l > (1ull << MAX_INDEX))
			return -1;

	int lvl = (int) l;


	if( l - lvl)
		return (lvl+1)>>1;

	return lvl>>1;		

}

void *
getBlock(int buddyIndex)
{
	int i;
	//ncPrint("searching for blocks on level %d\n",buddyIndex);
	//ncPrint("max: %x\n",max[buddyIndex]);
	for (i=0; i < level_size(i); i++)
	{
		uint64_t p = bitmaps[buddyIndex][i];
		//ncPrint("%x\t",p);
		if (p != 0)
		{
			uint64_t cmp = CMP;
			int j;
			for (j=0; j<32; j++)
			{
				//ncPrint("p: %"PRIx64"\n",p);
				if (cmp & p)
				{
					//bitmaps[buddyIndex][i] = p^cmp;
					//ncPrint("block found\n");	
					return dir(i,j,buddyIndex);
				}
				cmp= cmp>>2;
			}
		}
	}
	return (void*)0xDEAD;
}

void
panic(char* msg)
{
	ncPrint(msg);
	ncNewline();
	while(1);
	//exit(1);
}

void*
dir(int i, int offset, int level)
{
	uint64_t entry =  i*32 + offset;
	//ncPrint("level: %d, entry: %d\n",entry,level);
	uint64_t p = entry * block_size(level);
	return (void*)p;
}

void
set(int level, void* p, int val1, int val2)
{
	//ncPrint("called set val1: %d, val2: %d\n lvl: %d, addr:%x\n",val1,val2,level,p);
	uint64_t bit, blockIndex, i, offset, block, cmp = CMP, size = 32;
	int test2;
	double test1;
	
	test1 = (uint64_t)p/block_size(level);
	test2 = (int) test1;
		
	if(test1-test2){
		panic("Direccion incorrecta 1");
	}
	
	blockIndex = getBlockIndex(p,level);
	
	i = blockIndex/size;
	offset = blockIndex - i*size;
	
	//ncPrint("addr: %"PRIx64"\n",p);	
	//ncPrint("block: %"PRId64", i: %"PRId64", offset: %"PRId64"\n",blockIndex,i,offset);

	block = bitmaps[level][i];
	
	//ncPrint("block: %"PRIx64"\n",block);
	cmp = cmp >> (offset*2);
	//ncPrint("cmp: %"PRIx64"\n",cmp);
	bit = (cmp & block);
	//ncPrint("bit occupied: %d, val: %d\n",bit,val1);
	if (val1 != bit)
	{
		//ncPrint("changing occupied bit\n");
		bitmaps[level][i] = block ^ (cmp);
		block = bitmaps[level][i];
	}


	//ncPrint("block: %"PRIx64"\n",block);
	cmp = cmp >> 1;
	//ncPrint("cmp: %"PRIx64"\n",cmp);
	bit = (cmp & block);
	//ncPrint("bit part: %d, val: %d\n",bit,val2);
	
	if (val2 != bit)
	{
		//ncPrint("changing part bit\n");
		bitmaps[level][i] = block ^ (cmp);
	}	
	//block = bitmaps[level][i];
	//ncPrint("block: %x\n",block);
	
}

uint64_t 
getBlockIndex(void * p, int level)
{
	uint64_t index;

	index = (uint64_t)p/PAGE;
	index/=(1ull << level);	
	//ncPrint("index: %"PRId64"\n",index);	
	
	return index;
}



void 
printMap()
{
	ncClear();
	int i,j;
	for (i=0 ; i<MAX_LEVEL ; i++)
	{ 
		ncPrint("level: ");
		ncPrintDec(i);
		ncNewline();
		for(j=0 ; j<1; j++)
			ncPrintHex(bitmaps[i][j]);
		ncNewline();
	}
}

void
myfree(void* p)
{
	//ncPrint("free memory addr: %"PRIx64"\n",p);
	int t = freeMemory(p - HEAP_START,MAX_INDEX);

}
/*
int 
freeMemory(void * p, int level)
{
		int bud1, bud2, test;
		void* buddyAddr;
		
		test = (uint64_t) p / block_size[level];
			
		
		// ver si la dirección existe en el nivel
		if( p - test*block_size[level])
		{	
				//si la dirección no existe entonces se baja un nivel
				bud2 = freeMemory(p,level-1);
				buddyAddr = buddy(p,level);;
				bud1 = getStatus(buddyAddr,level);
				
				//si la memoria del hijo bud 2 está libre
				if(bud2 == 1)
				{
					//pregunto por el buddy 
					// si el buddy esta libre
					if(bud1 == 2)
					{
						//uno los bitmaps
						set(level,p,0,0);
						set(level,buddyAddr,0,0);
						return 1;
					}
					// si el buddy esta ocupado seteo bud2 como libre
					// bud1 no puede ser parte del mismo bloque que bud 1 ya que la dirección inicial p no existe en este nivel
					set(level,p,1,0);
				}	
				//como al menos uno de los dos hijos está ocupado el padre estará ocupado
				return 0;
		}
		//la dirección existe en este nivel
		// pregunto por el estado del nodo
		bud1 = getStatus(p,level);		
		buddyAddr = buddy(p,level);
		//pregunto por el estado del buddy
		bud2 = getStatus(buddyAddr,level);

		//si el nivel no es 0 y el nodo vale 1
		if(level && bud1 == 1)
		{
			//llamada recursiva al hijo izquierdo 
			//si el hijo izquierdo quedo libre
			if(freeMemory(left(p,level),level-1))
			{
				//si el buddy está libre o pertenece al padre
				if(bud2 == 0 || bud2 == 2)
				{
					//si el nivel no es el máximo (el nodo tiene buddy)
					if(level != MAX_INDEX)
					{	
						//seteo los bitmaps como parte del bloque padre			
						set(level,p,0,0);
						set(level,buddyAddr,0,0);
					}
					else
					{
						//es el máximo nivel (no hay buddy) seteo como libre
						set(level,p,1,0);
					}
					return 1;
				}
				set(level,p,1,0);			
							
			}
			// si el hijo izquierdo no está libre no tiene sentido preguntar por el derecho
			//con que un hijo no esté libre el padre tampoco lo estará			
			return 0;
		}
		//si el nivel es 0 o el nodo pertenece al padre
		if(!level || bud1 == 0)
		{		
			//si el buddy está libre o pertenece al padre
			if(bud2 == 0 || bud2 == 2)
			{	
				//si el nivel no es el máximo (el nodo tiene buddy)
				if(level != MAX_INDEX)
				{	
					//seteo los bitmaps como parte del bloque padre			
					set(level,p,0,0);
					set(level,buddyAddr,0,0);
				}
				else
				{
					//es el máximo nivel (no hay buddy) seteo como libre
					set(level,p,1,0);
				}
				return 1;
			}
			//si el nivel no es 0
			else if(level)
			{
				//llamada recursiva al hijo derecho
				//si el hijo derecho está libre
				if(freeMemory(buddyAddr,level-1))
				{
					//marco como parte del bloque padre
					set(level,p,0,0);
					set(level,buddyAddr,0,0);
					return 1;
				}
				else
				{
					//marco como libre
					set(level,p,1,0);
					return 0;
				}
			}

			set(level,p,1,0);
			return 0;
			
		}
		//la dirección que me pasaron no está ocupada
		if(bud1 == 2)
		{	
			panic("la dirección está libre");
			return -1;
		}
		
}
*/
int freeMemory(void* p, int level)
{

	int bud1, bud2, test, flag;
	void* buddyAddr;
	
	test = (uint64_t) p / block_size(level);	
	buddyAddr = buddy(p,level);;
	bud2 = getStatus(buddyAddr,level);

	// ver si la dirección existe en el nivel
	if( p - test*block_size(level))
	{	
		//si la dirección no existe entonces se baja un nivel
		bud1 = freeMemory(p,level-1);
		flag = 0;		
	}
	else
	{
		//si la dirección existe en el nivel me fijo su estado
		bud1 = getStatus(p,level);
		flag = 1;
	}
		
	switch(bud1)
	{
		case 0:
			if(flag)
			{
				if(level && (bud2 == 1) && freeMemory(buddyAddr,level - 1))
				{
					bud2 == 2;
				}
				if(bud2 == 0 || bud2 == 2)
				{
					if(level != MAX_LEVEL)
					{
						set(level,p,0,0);
						set(level,buddyAddr,0,0);
						return 1;
					}
					else
					{
						set(level,p,1,0);						
					}
				}
				set(level,p,1,0);
			}
			return 0;
		case 1:

			if(!level || !flag || freeMemory(p,level-1))
			{
				if(bud2 == 0 || bud2 == 2)
				{
					if(level != MAX_LEVEL)
					{
						set(level,p,0,0);
						set(level,buddyAddr,0,0);
					}
					else
					{
						set(level,p,1,0);
					}
					return 1;
				}
				set(level,p,1,0);
			}
			return 0;
	
		case 2:
		case -1:
			return -1;
		

	}



}







void *
buddy(void * p,int level)
{
	int i;
	i = (uint64_t)p/block_size(level);
	if(i%2)
	{	
		return (void*)(p - block_size(level));
	}
	return (void*)p + block_size(level);
}

int 
getStatus(void* p, int level)
{
		
	uint64_t bit, blockIndex, i, offset, block, cmp = 0xC000000000000000, size = 32;
	int test2;
	double test1;
	
	test1 = (uint64_t)p/block_size(level);
	test2 = (int) test1;	
	if(test1!=test2)
		panic("Direccion incorrecta");

	blockIndex = getBlockIndex(p,level);
	i = blockIndex/size;
	offset = blockIndex - i*size;

	block = bitmaps[level][i];
	
	cmp = cmp >> (offset*2);
	bit = (cmp & block);
	bit = bit >> (31 -offset)*2;		
	
	return bit;	

}


