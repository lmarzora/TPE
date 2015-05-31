GLOBAL horario
GLOBAL outb

section .text

horario:
	push ebp
	mov ebp, esp

	mov eax, 4
	out 70h, al
	in al, 71h

;	in dword [esp+8], 71h

;	out 70h, 2
;	in dword [esp+12], 71h
;	out 70h, 4
;	in dword [esp+16], 71h

	leave
	ret

outb:
	push ebp
	mov ebp, esp

	pushad

	mov word ax, [esp+8]
	mov byte bl, [esp+12]

	out ax, bl

	popad
	leave
	ret

inb:
	push ebp
	mov ebp, esp

	pushad

	mov word bx, [esp+8]

	mov eax, 0
	in al, bx 