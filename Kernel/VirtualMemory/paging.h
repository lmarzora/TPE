#include<stdint.h>
#include<lib.h>

#define Gib 0x40000000
#define PAGE 0x1000
#define PML4_MASK 	0xFF8000000000ULL
#define PDPT_MASK 	0x7FC0000000
#define PDT_MASK  	0x3FE00000
#define PT_MASK  	0x1FF000
#define P_4KiB_MASK 0xFFF
#define P_2MiB_MASK 0xFFFFF
#define P_1GiB_MASK 0xFFFFFFF
#define PML4_SHIFT 39
#define PDPT_SHIFT 30
#define PDT_SHIFT  21
#define PT_SHIFT   12



uint64_t getCR3();
void setCR3(uint64_t);


void setPresent(bool value, uint64_t* contents);
void setWritable(bool value, uint64_t* contents);

void setUser(bool value, uint64_t* contents);

void setPageWriteThrough(bool value, uint64_t* contents);

void setPageCacheDisable(bool value, uint64_t* contents);
void setAccessed(bool value, uint64_t* contents);

void setDirty(bool value, uint64_t* contents);

void setPageSize(bool value, uint64_t* contents);

void setGlobal(bool value, uint64_t* contents);

void setPageAttribueTable(bool value, uint64_t* contents);

void set4KiBPageAddress(void * address, uint64_t* contents);

void set2MiBPageAddress(void * address, uint64_t* contents);

void set1GiBPageAddress(void * address, uint64_t* contents);

void setExecuteDisable(bool value, uint64_t* contents);

bool getPresent(uint64_t* contents);

bool getWritable(uint64_t* contents);

bool getUser(uint64_t* contents);

bool getPageWriteThrough(uint64_t* contents);

bool getPageCacheDisable(uint64_t* contents);

bool getAccessed(uint64_t* contents);

bool getDirty(uint64_t* contents);

bool getPageSize(uint64_t* contents);
bool getGlobal(uint64_t* contents);

bool getPageAttribueTable(uint64_t* contents);
void * get4KiBPageAddress(uint64_t* contents);

void * get2MiBPageAddress(uint64_t* contents);
void * get1GiBPageAddress(uint64_t* contents);

bool getExecuteDisable(bool value,uint64_t* contents);

void clear(uint64_t* contents);

uint64_t* getPML4Offset(uint64_t vaddr);
uint64_t* getPDPTOffset(uint64_t vaddr);
uint64_t* getPDTOffset(uint64_t vaddr);
uint64_t* getPTOffset(uint64_t vaddr);
uint64_t* getP4KiBOffset(uint64_t vaddr);
uint64_t* getP2MiBOffset(uint64_t vaddr);
uint64_t* getP1GiBOffset(uint64_t vaddr);

void flushTable(uint64_t * table);

