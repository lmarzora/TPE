#include <stdint.h>
#include <naiveConsole.h>
#include <handlers.h>


void irqDispatcher(int64_t irq) {


	switch (irq) {
		case 0:
			ncPrint("tick");
			break;
		case 1:
			ncPrint("key");
			ncNewline();
			break;
		default:
			ncPrint("default");
	}
	return;
}
 
