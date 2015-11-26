#include"paging.h"
#include"pAllocator.h"
#include<lib.h>
#include<vmemory.h>

void setUpPaging()
{
	
	//set up PML4 and PDPT
	uint64_t* pml4 = alloc_page();
	uint64_t* pdpt = alloc_page();

	ncPrint("pml4: ");
	ncPrintHex(pml4);
	ncPrint(" pdpt: ");
	ncPrintHex(pdpt);
	ncNewline();
	memset(pml4,0,PAGE);
	memset(pdpt,0,PAGE);
	
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


	setCR3((uint64_t)pml4);
	ncNewline();
	ncPrint("CR3: ");
	ncPrintHex(getCR3());
	ncNewline();
}

int alloc_pMemory(uint64_t vMemory, int size, int user)
{
	uint64_t *page, *pdpt, *pdt, *pt ;
	int i, pdpt_i, pdt_i, pt_i, p_i;

	uint64_t* pml4 = getCR3();

	for(i=size ; i>0 ; i = i - PAGE) { 

		pdpt_i = getPDPT(vMemory);
		pdt_i = getPDT(vMemory);
		pt_i = getPT(vMemory);
		p_i = getP(vMemory);
		
		if(!getPresent(&pml4[pdpt_i])) {
			page = alloc_page();
			map4KibPage(page, &pml4[pdpt_i], user);	
		}
		pdpt = pml4[pdpt_i];

		if(!getPresent(&pdpt[pdt_i])) {
			page = alloc_page();
			map4KibPage(page, &pdpt[pdt_i], user);	
		}
		pdt = pdpt[pdt_i];

		if(!getPresent(&pdt[pt_i])) {
			page = alloc_page();
			map4KibPage(page, &pdt[pt_i], user);	
		}
	
		pt = pdt[p_i];

		page = alloc_page();
		map4KibPage(page, &pt[p_i], user);

	}
	return 1;
}

int free_pMemory(uint64_t vMemory)
{
		int i, pdpt_i, pdt_i, pt_i, p_i;
		uint64_t* pml4 ,* pdpt,* pdt ,* pt ,* page;
		pml4 = getCR3();
		pdpt_i = getPDPT(vMemory);
		pdt_i = getPDT(vMemory);
		pt_i = getPT(vMemory);
		p_i = getP(vMemory);
		
		pdpt = &pml4[pdpt_i];
		pdt = &pdpt[pdt_i];
		pt = &pdt[pt_i];
		page = &pt[p_i];

		free_page(page);
		return 1;

}

int map4KibPage(uint64_t vMemory, uint64_t* pMemory, int user)
{
		setPresent(true,pMemory);
		setWritable(true,pMemory);
		setUser(user,pMemory);
		setPageWriteThrough(false,pMemory);
		setPageCacheDisable(false,pMemory);
		setAccessed(false,pMemory);
		setPageSize(false,pMemory);
		set4KiBPageAddress(vMemory,pMemory);

} 

uint64_t* getPDPT(uint64_t vaddr) {
	
	return (uint64_t*) (vaddr && PDPT_MASK);

}
uint64_t* getPDT(uint64_t vaddr) {
	
	return (uint64_t*) (vaddr && PDT_MASK);

}
uint64_t* getPT(uint64_t vaddr) {
	
	return (uint64_t*) (vaddr && PT_MASK);

}
uint64_t* getP(uint64_t vaddr) {
	
	return (uint64_t*) (vaddr && P_MASK);

}




