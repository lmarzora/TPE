#include "arch/interrupts.h"
#include "defs.h"
#include "arch/handlers.h"

int horario();

DESCR_INT idt[0xA];	// IDT de 11 entradas
IDTR idtr;			// IDTR

void setup_IDT_entry (int index, byte selector, dword offset, byte access);

/**********************************************
kmain() 
Punto de entrada de c�digo C.
*************************************************/

int kmain()
{

	setup_IDT_entry (0x08, 0x08, (dword)&_irq00Handler, ACS_INT);
	setup_IDT_entry (0x09, 0x08, (dword)&_irq01Handler, ACS_INT);


	idtr.base = 0;  
	idtr.base +=(dword) &idt;
	idtr.limit = sizeof(idt)-1;
	
	_lidt (&idtr);	

	
	//Todas las interrupciones desabilidas.
	picMasterMask(0xFC); 
	picSlaveMask(0xFF);
        
	_sti();

	clearScreen();

	int hora = horario();
	printInt(hora);
	printInt(2);

	while(1){
	}


}

void setup_IDT_entry (int index, byte selector, dword offset, byte access) {
  idt[index].selector = selector;
  idt[index].offset_l = offset & 0xFFFF;
  idt[index].offset_h = offset >> 16;
  idt[index].access = access;
  idt[index].cero = 0;
}



