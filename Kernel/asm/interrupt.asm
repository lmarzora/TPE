;PIT 
GLOBAL pit_handler
GLOBAL sti
GLOBAL keyboard_handler
EXTERN irqDispatcher
GLOBAL pic
GLOBAL getKey
GLOBAL int80handler
EXTERN syscall
EXTERN schedule
EXTERN ncPrintHex
EXTERN ncNewline

%macro irqHandlerMaster 1
	mov rdi, %1
	call irqDispatcher
	

	;signal pic	

	mov al, 20h
	out 20h, al
	iretq
%endmacro



%macro	pushState 0
	push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
	push fs
	push gs
%endmacro

%macro	popState 0
	pop gs
	pop fs
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	pop rax

%endmacro




keyboard_handler:
	irqHandlerMaster 1
pit_handler:
	pushState

	mov rdi, rsp

	call schedule

	mov rsp, rax

	
	
	popState
	


	
	iretq

int80handler:
;	sti
	call syscall
	iretq

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
	mov al, 0xfc
	out 0x21, al
	mov al, 0xff
	out 0xA1, al
	sti
	ret


getFlags:
	pushf
	pop ax;
	ret

