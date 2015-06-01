#include "types.h"
#include "klib.h"
void intKeyboard();

void irqDispatcher(dword irq){
	switch(irq) {
		case 0:
			//int_08();
			//println("Tick");
			toggleCursor();
			break;
		case 1:
			intKeyboard();
			break;
	}
	return;
}


static int i = 0;

void int_08(){
	char *video = (char *) 0xb8000;
	video[i++] = (char) i;
}

void intKeyboard(){
	insertKeyboard(getKey());
}