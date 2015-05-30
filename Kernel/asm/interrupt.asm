;PIT 
GLOBAL pit_handler
GLOBAL sti
GLOBAL keyboard_handler
EXTERN irqDispatcher

%macro irqHandlerMaster 1
	mov rdi, %1
	call irqDispatcher
	
	;signal pic
	mov al, 20h
	out 20h, al

	iret
%endmacro


keyboard_handler:
	irqHandlerMaster 1
pit_handler:
	irqHandlerMaster 0

sti:
	sti
	ret
