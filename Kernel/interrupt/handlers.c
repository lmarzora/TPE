#include <handlers.h>
#include <keyboard.h>
#include <terminal.h>
#include <naiveConsole.h>
#include <sysCallAttention.h>


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
		default:
			break;
	}
	return dim;
}

 
