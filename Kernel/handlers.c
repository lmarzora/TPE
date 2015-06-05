#include <stdint.h>
#include <naiveConsole.h>
#include <handlers.h>
#include <keyboard.h>


void irqDispatcher(int64_t irq) {


	switch (irq) {
		case 0:
			ncPrint("tick");
			incTicker();
			break;
		case 1:
			analizeKeyboard(getKey());
			//ncNewline();
			break;
		default:
			ncPrint("default");
	}
	return;
}

int syscall(int code , char* buff , int size) {
	int dim = -1;
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
		default:
			ncPrint("AAAAAAAAAAAAAAAAAAAAAAAAAAA");	
	}
	return dim;
}

 
