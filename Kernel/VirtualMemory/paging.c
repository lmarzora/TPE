#include"paging.h"
#include<lib.h>
#include<vmemory.h>

void setUpPaging()
{

	//set up PML4 and PDPT
	uint64_t* pml4 = kalloc(0x1000,0);
	uint64_t* pdpt = kalloc(0x1000,0);
	
	setPresent(true,pml4);
	setWritable(true,pml4);
	setUser(true,pml4);
	setPageWriteThrough(false,pml4);
	setPageCacheDisable(false,pml4);
	setAccessed(false,pml4);
	setPageSize(false,pml4);
	setExecuteDisable(false,pml4);
	pml4[0] = set4KiBPageAddress(pdpt,pml4[0]);

	//set 1 1GiB page for kernel

	void* addr= 0; 

	setPresent(true,pdpt);
	setWritable(true,pdpt);
	setUser(false,pdpt);
	setPageWriteThrough(false,pdpt);
	setPageCacheDisable(false,pdpt);
	setAccessed(false,pdpt);
	setPageSize(true,pdpt);
	pdpt[0] = set1GiBPageAddress(addr,pdpt[0]);


	//set 2 1GiB userland pages

	addr+=Gib;

	setPresent(true,&pdpt[1]);
	setWritable(true,&pdpt[1]);
	setUser(true,&pdpt[1]);
	setPageWriteThrough(false,&pdpt[1]);
	setPageCacheDisable(false,&pdpt[1]);
	setAccessed(false,&pdpt[1]);
	setPageSize(true,&pdpt[1]);
	pdpt[1] = set1GiBPageAddress(addr,pdpt[1]);

	addr+=Gib;

	setPresent(true,&pdpt[2]);
	setWritable(true,&pdpt[2]);
	setUser(true,&pdpt[2]);
	setPageWriteThrough(false,&pdpt[2]);
	setPageCacheDisable(false,&pdpt[2]);
	setAccessed(false,&pdpt[2]);
	setPageSize(true,&pdpt[2]);
	pdpt[2] = set1GiBPageAddress(addr, pdpt[2]);

	//set cr3

	setCR3(pml4);

}



