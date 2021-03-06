#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <handlers.h>
#include <idt.h>
#include <scheduler_interface.h>
#include "kSetUp.h"
#include <semaphore.h>
#include <msgqueue.h>
#include <vmemory.h>

#define TOTAL_MEMORY 0x100000000

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


void nullProc();
void garbageProc();
void beyondStack();

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
	init_pMemoryAllocator(TOTAL_MEMORY);
	setUpPaging();
	
	idt_set_gate(0x0E,(uint64_t)pageFaultHandler,0x8,0x8E);

	idt_set_gate(0x08,(uint64_t)doubleFaultHandler,0x8,0x8E);


	setUpPageFrameAllocator(memory);
	
	setUpScheduler();
	

	//map user code module
	void* userland = setUserModule(sampleCodeModuleAddress);

	process_data* shell = kalloc(sizeof(process_data),0);
	shell->func = userland;
	shell->name = "shell";
	shell->isForeground = 1;
	newProcess(shell);	
	


	process_data* garbage = kalloc(sizeof(process_data),0);

	
	garbage->func = &garbageProc;
	garbage->name = "garbage_col";
	
	newProcess(garbage);

	process_data* null_task = kalloc(sizeof(process_data),0);

	
	null_task->func = &nullProc;
	null_task->name = "kill_me";
	
	newProcess(null_task);

	//ncNewline();
	//ncClear();

	//while(1);
	idt_set_gate(0x20,(uint64_t)pit_handler,0x8,0x8E);
	idt_set_gate(0x21,(uint64_t)keyboard_handler,0x8,0x8E);
	idt_set_gate(0x80,(uint64_t)int80handler,0x8,0x8F);
	idt_set_gate(0x81,(uint64_t)int81handler,0x8,0x8F);
	idt_set_gate(0x82,(uint64_t)int82handler,0x8,0x8F);
	idt_set_gate(0x83,(uint64_t)int83handler,0x8,0x8F);
	idt_set_gate(0x84,(uint64_t)int84handler,0x8,0x8F);
	
	//FncClear();
	ncClear();
	sti();
	pic();

	
	//((EntryPoint)sampleCodeModuleAddress)();


	return 0;
}

void garbageProc(){
	while(1){
		freeTerminated();
		halt();
	}
}
void nullProc(){
	while(1);
}




