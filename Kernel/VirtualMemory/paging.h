#include<stdint.h>
#include<lib.h>

#define Gib 0x40000000
#define PDPT_MASK 0x7FC0000000
#define PDT_MASK  0x3FE00000
#define PT_MASK   0x1FF000
#define P_MASK    0xFFF

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

uint64_t set4KiBPageAddress(void * address, uint64_t);

uint64_t set2MiBPageAddress(void * address, uint64_t);

uint64_t set1GiBPageAddress(void * address, uint64_t);

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


uint64_t* getPDPT(uint64_t vaddr);
uint64_t* getPDT(uint64_t vaddr);
uint64_t* getPT(uint64_t vaddr);
uint64_t* getP(uint64_t vaddr);

