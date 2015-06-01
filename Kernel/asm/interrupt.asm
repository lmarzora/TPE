;PIT 
GLOBAL pit_handler
GLOBAL sti
GLOBAL keyboard_handler
EXTERN irqDispatcher
EXTERN ncNewline
GLOBAL pic
GLOBAL getKey

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

getKey:
;	push ebp
;	mov ebp,esp
	mov rax, 0
;	mov eax,0
;	in ax,60h
	in  ax, 60h
;	leave
	ret

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