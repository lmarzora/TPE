GLOBAL toggleCursor

section .text

toggleCursor:
	mov al, 0x0F
	mov dx, 0X3D4
	out dx, al

	mov rax, rdi
	mov dx, 0X3D5
	out dx, al

	mov al, 0x0E
	mov dx, 0X3D4
	out dx, al

	mov rax, rsi
	mov dx, 0X3D5
	out dx, al

	ret