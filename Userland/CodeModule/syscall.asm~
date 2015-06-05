GLOBAL read
GLOBAL write

read:
	mov rdx, rsi
	mov rsi, rdi
	mov	rdi, 1
	int 80h
	ret

write:
	mov rdx, rsi
	mov rsi, rdi
	mov	rdi, 2
	int 80h
	ret
