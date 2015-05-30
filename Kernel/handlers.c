#include <stdint.h>
#include <naiveConsole.h>
#include <handlers.h>


void irqDispatcher(int64_t irq) {
	ncNewline();
	ncPrintDec(irq);
	ncPrint("interrupcion");
	ncNewline();

	switch (irq) {
		case 0:
			ncPrint("tick");
			ncNewline();
			break;
		case 1:
			ncNewline();
			break;
		default:
			ncPrint("default");
	}
	return;
}
 
