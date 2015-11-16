#include<stdint.h>
#include<inttypes.h>

#define PAGE 0x1000
void mem_setup(unsigned himem_size);
int block_size(int level);
int level_size(int level);
void * 
myalloc(uint64_t bytes);
int
reserveMemory(void* p, uint64_t bytes, int level);
int 
freeMemory(void* p , int level);
void*
left(void* p, int level);
void*
right(void*p, int level);
int
getLevel(uint64_t bytes);
void *
getBlock(int buddyIndex);
void
panic(char* msg);
void*
dir(int i, int j, int level);
void
set(int level, void*p , int val1, int val2);
void 
printMap();
void
printLevels();
void
availBlocks(int level);
uint64_t 
getBlockIndex(void * p, int level);
void*
buddy(void* p, int level);
int 
getStatus(void* p, int level);
void *
buddy(void*p,int level);
void
myfree(void* p);
