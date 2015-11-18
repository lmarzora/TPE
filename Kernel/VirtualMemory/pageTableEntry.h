#include<stdint.h>
#include<lib.h>

void setBit(uint64_t* bits, uint32_t n, bool value);
bool getBit(uint64_t* bits, uint32_t n);


	static const uint32_t presentBit = 0;
	static const uint32_t writeBit = 1;
	static const uint32_t userBit = 2;
	static const uint32_t pageWriteThroughBit = 3;
	static const uint32_t pageCacheDisableBit = 4;
	static const uint32_t accessedBit = 5;
	static const uint32_t dirtyBit = 6;
	static const uint32_t pageSizeBit = 7;
	static const uint32_t globalBit = 8;
	static const uint32_t pageAttributeTableBit = 12;
	static const uint32_t addressBit = 30;
	static const uint64_t addressMask4KiB = 0x0000000FFFFFFFFF000;
	static const uint64_t addressMask2MiB = 0x0000000FFFFFFE00000;
	static const uint64_t addressMask1GiB = 0x0000000FFFFC0000000;
	static const uint32_t executeDisableBit = 63;
	static const uint64_t pageSize = 4092;

