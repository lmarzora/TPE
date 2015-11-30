#include"paging.h"
#include"pAllocator.h"
#include<lib.h>
#include<vmemory.h>

void setUpPaging()
{
	
	//set up PML4 and PDPT
	uint64_t* pml4 = 0x510000;
	uint64_t* pdpt = 0x520000;

	ncPrint("pml4: ");
	ncPrintHex(pml4);
	ncPrint(" pdpt: ");
	ncPrintHex(pdpt);
	ncNewline();
	flushTable(pml4);
	flushTable(pdpt);
	
	setPresent(true,pml4);
	setWritable(true,pml4);
	setUser(true,pml4);
	setPageWriteThrough(false,pml4);
	setPageCacheDisable(false,pml4);
	setAccessed(false,pml4);
	setPageSize(false,pml4);
	setExecuteDisable(false,pml4);
	set4KiBPageAddress(pdpt,&pml4[0]);

	//set 1 1GiB page for kernel

	uint64_t* addr= 0; 

	setPresent(true,pdpt);
	setWritable(true,pdpt);
	setUser(false,pdpt);
	setPageWriteThrough(false,pdpt);
	setPageCacheDisable(false,pdpt);
	setAccessed(false,pdpt);
	setPageSize(true,pdpt);
	set1GiBPageAddress(addr,&pdpt[0]);


	setCR3((uint64_t)pml4);
	ncNewline();
	ncPrint("CR3: ");
	ncPrintHex(getCR3());
	ncNewline();
}

void mapUserModule(uint64_t* vAddress, uint64_t* pAddress)
{
	uint64_t *page, *pdpt, *pdt, *pt ;
	int i, pml4_i, pdpt_i, pdt_i, pt_i;

	uint64_t* pml4 = getCR3();

	pml4_i = getPML4Offset(vAddress);
	pdpt_i = getPDPTOffset(vAddress);
	pdt_i = getPDTOffset(vAddress);

	if(!getPresent(&pml4[pml4_i])) {
		page = alloc_page();
		flushTable(page);
		map4KibPage(page, &pml4[pml4_i], 1);
	}

	pdpt = get4KiBPageAddress(&pml4[pml4_i]);

	if(!getPresent(&pdpt[pdpt_i])) {	
		page = alloc_page();
		flushTable(page);
		map4KibPage(page, &pdpt[pdpt_i], 1);
	}
			
	pdt = get4KiBPageAddress(&pdpt[pdpt_i]);

	

	setPresent(true,&pdt[pdt_i]);
	setWritable(true,&pdt[pdt_i]);
	setUser(true,&pdt[pdt_i]);
	setPageWriteThrough(false,&pdt[pdt_i]);
	setPageCacheDisable(false,&pdt[pdt_i]);
	setAccessed(false,&pdt[pdt_i]);
	setPageSize(true,&pdt[pdt_i]);
	set2MiBPageAddress(pAddress,&pdt[pdt_i]);
	

}


int alloc_pMemory(uint64_t vMemory, int size, int user)
{
	//ncPrint("ALLOC PAGES\n");
	uint64_t *page, *pdpt, *pdt, *pt ;
	int i, pml4_i, pdpt_i, pdt_i, pt_i;

	uint64_t* pml4 = getCR3();
	//ncPrint("cr3\n");
	//ncNewline();

	for(i=size ; i>0 ; i = i - PAGE) { 
	//	ncPrintDec(i);
	//	ncNewline();
		pml4_i = getPML4Offset(vMemory);
		pdpt_i = getPDPTOffset(vMemory);
		pdt_i = getPDTOffset(vMemory);
		pt_i = getPTOffset(vMemory);


		if(!getPresent(&pml4[pml4_i])) {
			
	//		ncPrint("setting PDPT ");
			page = alloc_page();
	//		ncPrintHex(page);
	//		ncPrint(" ");
			flushTable(page);
			map4KibPage(page, &pml4[pml4_i], 1);
	//		ncPrintHex(get4KiBPageAddress(&pml4[pml4_i]));
	//		ncNewline();	
		}

		pdpt = get4KiBPageAddress(&pml4[pml4_i]);


		if(!getPresent(&pdpt[pdpt_i])) {
	//		ncPrint("setting PDT\n");
			page = alloc_page();
	//		ncPrintHex(page);
	//		ncPrint(" ");
			flushTable(page);
			map4KibPage(page, &pdpt[pdpt_i], 1);
	//		ncPrintHex(get4KiBPageAddress(&pdpt[pdpt_i]));
	//		ncNewline();		
		}
			
		pdt = get4KiBPageAddress(&pdpt[pdpt_i]);

		if(!getPresent(&pdt[pdt_i])) {
	//		ncPrint("setting PT\n");
			page = alloc_page();
	//		ncPrintHex(page);
	//		ncPrint(" ");
			flushTable(page);
			map4KibPage(page, &pdt[pdt_i], 1);	
	//		ncPrintHex(get4KiBPageAddress(&pdt[pdt_i]));
	//		ncNewline();	
		}
	
		pt = get4KiBPageAddress(&pdt[pdt_i]);
	/*
		ncPrint("setting up P :");
		ncPrint(" pt_i: ");
		ncPrintDec(pt_i);
		ncPrint(" page: ");
	*/
		page = alloc_page();
	/*
		ncPrintHex(page);
		ncPrint(" page: ");
	*/
		map4KibPage(page, &pt[pt_i], user);
	/*
		ncPrintHex(get4KiBPageAddress(&pt[pt_i]));
		ncNewline();
	*/	
		vMemory+=PAGE;	
	

		
	}
			
	return 1;
}

int free_pMemory(uint64_t pMemory)
{
		free_page(pMemory);
		return 1;

}

int map4KibPage(uint64_t* pMemory, uint64_t* contents, int user)
{
		setPresent(true,contents);
		setWritable(true,contents);
		setUser(user,contents);
		setPageWriteThrough(false,contents);
		setPageCacheDisable(false,contents);
		setAccessed(false,contents);
		setPageSize(false,contents);
		set4KiBPageAddress(pMemory,contents);
		return 1;

} 

uint64_t* getPML4Offset(uint64_t vaddr) {
	return (uint64_t*) ((vaddr & PML4_MASK)>>PML4_SHIFT);
}


uint64_t* getPDPTOffset(uint64_t vaddr) {

	return (uint64_t*) ((vaddr & PDPT_MASK)>>PDPT_SHIFT);

}
uint64_t* getPDTOffset(uint64_t vaddr) {
	
	return (uint64_t*) ((vaddr & PDT_MASK)>>PDT_SHIFT);

}
uint64_t* getPTOffset(uint64_t vaddr) {
	
	return (uint64_t*) ((vaddr & PT_MASK)>>PT_SHIFT);

}

uint64_t* getP4KiBOffset(uint64_t vaddr) {
	
	return (uint64_t*) (vaddr & P_4KiB_MASK);

}
uint64_t* getP2MiBOffset(uint64_t vaddr) {
	
	return (uint64_t*) (vaddr & P_2MiB_MASK);

}
uint64_t* getP1GiBOffset(uint64_t vaddr) {
	
	return (uint64_t*) (vaddr & P_1GiB_MASK);

}


uint64_t* get_pAddress( uint64_t vMemory )
{
		//ncClear();
	/*
		ncPrint("getting physical address\n");
		ncPrintHex(vMemory);
		ncNewline();
	*/		
	
		uint64_t  *pml4, *pdpt, *pdt, *pt ;
		int  pml4_i, pdpt_i, pdt_i, pt_i, p_i;

		pml4 = getCR3();	
		ncNewline();		
		
		pml4_i = getPML4Offset(vMemory);
		pdpt_i = getPDPTOffset(vMemory);
		pdt_i = getPDTOffset(vMemory);
		pt_i = getPTOffset(vMemory);
	/*	
		ncPrint("pml4 : ");
		ncPrintHex(pml4);
		ncPrint(" i : ");		
		ncPrintDec(pml4_i);		
	*/
		if(!getPresent(&pml4[pml4_i]))
		{
			
			ncNewline();
			ncPrint("PDPT NOT PRESENT\n");
					
			return  -1;
		}


		if(getPageSize(&pml4[pml4_i]))
		{
			/*
			ncNewline();
			ncPrint("512 GiB\n");
			*/
			return (uint64_t*) get1GiBPageAddress(pdpt);
		}
		

		pdpt = (uint64_t*) get4KiBPageAddress(&pml4[pml4_i]);	
		
	/*
		ncPrint(" pdpt : "); 
		ncPrintHex(pdpt);
		ncPrint(" i : ");		
		ncPrintDec(pdpt_i);
	*/
		if(!getPresent(&pdpt[pdpt_i]))
		{
			ncNewline();
			ncPrint("PDT NOT PRESENT\n");
			return  -1;
		}


		if(getPageSize(&pdpt[pdpt_i]))
		{
			
			ncNewline();
			ncPrint("1 GiB\n");
			
			p_i =  getP1GiBOffset(vMemory);
			return (uint64_t*) (get1GiBPageAddress(pdpt) + p_i);
		}

		pdt = (uint64_t*) get4KiBPageAddress(&pdpt[pdpt_i]);
		
	/*
		ncPrint(" pdt : "); 
		ncPrintHex(pdt);
		ncPrint(" i : ");		
		ncPrintDec(pdt_i);	
	*/
		if(!getPresent(&pdt[pdt_i]))
		{
			ncNewline();
			ncPrint("PT NOT PRESENT\n");
			return  -1;
		}
		

		if(getPageSize(&pdt[pdt_i]))
		{
			/*	
			ncNewline();
			ncPrint("2 MiB\n");
			*/
			p_i =  getP2MiBOffset(vMemory);
			return (uint64_t*) (get2MiBPageAddress(pdt) + p_i);
		}


		pt = (uint64_t*)get4KiBPageAddress(&pdt[pdt_i]);

		if(!getPresent(&pt[pt_i]))
		{
			ncNewline();
			ncPrint("PAGE NOT PRESENT\n");
			return  -1;
		}
	/*
		ncPrint(" pt : "); 
		ncPrintHex(pt);
		ncPrint(" i : ");		
		ncPrintDec(pt_i);	
		ncNewline();
		ncPrintHex(get4KiBPageAddress(&pt[pt_i]));
		ncNewline();
		ncPrint("4 KiB\n");
		ncNewline();
	*/	
		p_i =  getP4KiBOffset(vMemory);
		return (uint64_t*) (get4KiBPageAddress(&pt[pt_i]) + p_i);

}


void flushTable(uint64_t * table)
{
	//ncPrint("FLUSH\n");
	memset(table,0,PAGE);
}



