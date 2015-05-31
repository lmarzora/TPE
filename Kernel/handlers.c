#include <stdint.h>
#include <naiveConsole.h>
#include <handlers.h>


void irqDispatcher(int64_t irq) {
	ncNewline();
	ncPrint("interrupcion");
	ncNewline();

	switch (irq) {
		case 0:
			ncPrint("tick");
			ncNewline();
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
 
