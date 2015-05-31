;PIT 
GLOBAL pit_handler
GLOBAL sti
GLOBAL keyboard_handler
EXTERN irqDispatcher
EXTERN ncNewline
GLOBAL pic

%macro irqHandlerMaster 1
	mov rdi, %1
	call irqDispatcher
	

	;signal pic	

	mov al, 20h
	out 20h, al
	iretq
%endmacro


keyboard_handler:
	irqHandlerMaster 1
pit_handler:
	irqHandlerMaster 0

sti:
	sti
	ret

pic:	
	mov al, 0xfd
	out 0x21, al
	mov al, 0xff
	out 0xA1, al
	sti
	ret
