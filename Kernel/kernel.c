#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <handlers.h>
#include <idt.h>
#include <scheduler_interface.h>
#include "kSetUp.h"

#define TOTAL_MEMORY 0x10000000000000 

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static void * const sampleCodeModuleAddress = (void*)0x400000;
static void * const sampleDataModuleAddress = (void*)0x500000;

typedef int (*EntryPoint)();


void tareaNula(void);

void clearBSS(void * bssAddress, uint64_t bssSize)
{
	memset(bssAddress, 0, bssSize);
}

void * getStackBase()
{
	return (void*)(
		(uint64_t)&endOfKernel
		+ PageSize * 8				//The size of the stack itself, 32KiB
		- sizeof(uint64_t)			//Begin at the top of the stack
	);
}

void * initializeKernelBinary()
{
	char buffer[10];

	ncPrint("[x64BareBones]");
	ncNewline();

	ncPrint("CPU Vendor:");
	ncPrint(cpuVendor(buffer));
	ncNewline();

	ncPrint("[Loading modules]");
	ncNewline();
	void * moduleAddresses[] = {
		sampleCodeModuleAddress,
		sampleDataModuleAddress
	};

	loadModules(&endOfKernelBinary, moduleAddresses);
	ncPrint("[Done]");
	ncNewline();
	ncNewline();

	ncPrint("[Initializing kernel's binary]");
	ncNewline();

	clearBSS(&bss, &endOfKernel - &bss);

	ncPrint("  text: 0x");
	ncPrintHex((uint64_t)&text);
	ncNewline();
	ncPrint("  rodata: 0x");
	ncPrintHex((uint64_t)&rodata);
	ncNewline();
	ncPrint("  data: 0x");
	ncPrintHex((uint64_t)&data);
	ncNewline();
	ncPrint("  bss: 0x");
	ncPrintHex((uint64_t)&bss);
	ncNewline();

	ncPrint("[Done]");
	ncNewline();
	ncNewline();
	return getStackBase();
}

int main()
{	

	uint64_t memory = TOTAL_MEMORY;
	

	setUpPageAllocator(memory);
	setUpScheduler();
	ncClear();
	
	process_data* shell = kalloc(sizeof(process_data),0);

	
	shell->func = sampleCodeModuleAddress;
	shell->name = "shell";
	
	newProcess(shell);	


	process_data* null_task = kalloc(sizeof(process_data),0);

	
	null_task->func = &tareaNula;
	null_task->name = "tarea_nula";
	
	newProcess(null_task);


	ncNewline();
	//ncClear();

	idt_set_gate(0x20,(uint64_t)pit_handler,0x8,0x8E);
	idt_set_gate(0x21,(uint64_t)keyboard_handler,0x8,0x8E);
	idt_set_gate(0x80,(uint64_t)int80handler,0x8,0x8F);
	
	//FncClear();
		
	sti();
	pic();;

	
	//((EntryPoint)sampleCodeModuleAddress)();


	return 0;
}

void tareaNula(void)
{
	while(1);
}


