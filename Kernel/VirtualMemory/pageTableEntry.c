#include"pageTableEntry.h"


void setBit(uint64_t* bits, uint32_t n, bool value) {
	
	if (value)
		(*bits) |= (1ull << n);
	else
		(*bits) &= ~(1ull << n);
}

bool getBit(uint64_t* bits, uint32_t n) {
	return (*bits) & (1ull << n);
}

void setPresent(bool value, uint64_t* contents) {
	setBit(contents, presentBit, value);
}

void setWritable(bool value, uint64_t* contents) {
	setBit(contents, writeBit, value);
}

void setUser(bool value, uint64_t* contents) {
	setBit(contents, userBit, value);
}

void setPageWriteThrough(bool value, uint64_t* contents) {
	setBit(contents, pageWriteThroughBit, value);
}

void setPageCacheDisable(bool value, uint64_t* contents) {
	setBit(contents, pageCacheDisableBit, value);
}

void setAccessed(bool value, uint64_t* contents) {
	setBit(contents, accessedBit, value);
}

void setDirty(bool value, uint64_t* contents) {
	setBit(contents, dirtyBit, value);
}

void setPageSize(bool value, uint64_t* contents) {
	setBit(contents, pageSizeBit, value);
}

void setGlobal(bool value, uint64_t* contents) {
	setBit(contents, globalBit, value);
}

void setPageAttribueTable(bool value, uint64_t* contents) {
	setBit(contents, pageAttributeTableBit, value);
}

uint64_t set4KiBPageAddress(void * address, uint64_t contents) {
	return ((uint64_t)address & addressMask4KiB) | (contents & ~addressMask4KiB);
}

uint64_t set2MiBPageAddress(void * address, uint64_t contents) {
	return ((uint64_t)address & addressMask2MiB) | (contents & ~addressMask2MiB);
}

uint64_t set1GiBPageAddress(void * address, uint64_t contents) {
	return ((uint64_t)address & addressMask1GiB) | (contents & ~addressMask1GiB);
}

void setExecuteDisable(bool value, uint64_t* contents) {
	setBit(contents, executeDisableBit, value);
}

bool getPresent(uint64_t* contents) {
	return getBit(contents, presentBit);
}

bool getWritable(uint64_t* contents) {
	return getBit(contents, writeBit);
}

bool getUser(uint64_t* contents) {
	return getBit(contents, userBit);
}

bool getPageWriteThrough(uint64_t* contents) {
	return getBit(contents, pageWriteThroughBit);
}

bool getPageCacheDisable(uint64_t* contents) {
	return getBit(contents, pageCacheDisableBit);
}

bool getAccessed(uint64_t* contents) {
	return getBit(contents, accessedBit);
}

bool getDirty(uint64_t* contents) {
	return getBit(contents, dirtyBit);
}

bool getPageSize(uint64_t* contents) {
	return getBit(contents, pageSizeBit);
}

bool getGlobal(uint64_t* contents) {
	return getBit(contents, globalBit);
}

bool getPageAttribueTable(uint64_t* contents) {
	return getBit(contents, pageAttributeTableBit);
}

void * get4KiBPageAddress(uint64_t* contents) {
	return (void*)(*contents & addressMask4KiB);
}

void * get2MiBPageAddress(uint64_t* contents) {
	return (void*)(*contents & addressMask2MiB);
}

void * get1GiBPageAddress(uint64_t* contents) {
	return (void*)(*contents & addressMask1GiB);
}

bool getExecuteDisable(bool value,uint64_t* contents) {
	return getBit(contents, executeDisableBit);
}

void clear(uint64_t* contents) {
	*contents=0;
}
