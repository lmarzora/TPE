GLOBAL getHora
GLOBAL getMin
GLOBAL getSec

GLOBAL setHora
GLOBAL setMin
GLOBAL setSec

%macro getTime 1
	;mov rdi, %1
	
	mov rax, 0

	mov al, %1
	out 70h, al

	mov al, 0
	in al, 71h

	ret
%endmacro

getHora:
	getTime 4

getMin:
	getTime 2

getSec:
	getTime 0


%macro setTime 1
	
	mov rax, 0;
	mov al, %1
	out 70h, al
	
	mov rax, 0
	mov rax, rdi
	out 71h, al

	ret
%endmacro

setHora:
	setTime 4

setMin:
	setTime 2

setSec:
	setTime 0



