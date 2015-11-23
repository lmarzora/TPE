#include <handlers.h>
#include <keyboard.h>
#include <terminal.h>
#include <naiveConsole.h>
#include <sysCallAttention.h>
#include "../semaphore.h"
#include "../msgqueue.h"
#include <scheduler_interface.h>

void irqDispatcher(int64_t irq) {


	switch (irq) {
		case 0:
			incTick();
			break;
		case 1:	
			resetTick();
			analizeKeyboard(getKey());
			break;
		default:
			break;
	}
	return;
}

int syscall(int code , char* buff , int size) {
	int dim = -1;
	void** buff2;
	process_data* data;
	MsgQueue * luis;

	switch (code) {
		case 1: 
			dim = sysread(buff,size);
			break;
		case 2:
			dim = syswrite(buff,size);
			break;
		case 3:
			sysGetTime();
			break;
		case 4:			
			sysSetTime(buff);
			break;
		case 5:
			clearScreen();
			break;
		case 6:
			sysSetInterval(size);
			break;
		case 7:
			ncPrintDec(size);
			getPages(size,buff);
			break;
		case 8:
			printProcesses();
			break;
		case 9:
			data = kalloc(sizeof(process_data),0);

	
			data->func = buff;
			data->name = "programa";
			newProcess(data);
			break;
		case 10:
			killProcess(size);
			break;
		case 23:
			/*
			luis = CreateSem("hola", 0);
			ncPrint("ANTESS\n");
			WaitSem(luis);
			ncPrint("DESPUES\n");*/
			luis = getMessageQueue("wachin");
			if(!luis){
				//ncPrint("No lo encontro\n");
			}else{
				//ncPrint("Lo encontro\n");
				PutMsgQueue(luis, "banana");
			}

			break;
		default:
			break;
	}
	return dim;
}

int setInterrupt(int valor){
	uint16_t flags = getFlags();
	flags = flags&0x200;
	flags = flags>>9;

	if(valor){
		sti();
	}else{
		cli();
	}

	return flags;
}
