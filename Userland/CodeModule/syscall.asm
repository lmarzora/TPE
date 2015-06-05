GLOBAL read
GLOBAL write
GLOBAL timeGet

read:
	mov rdx, rsi
	mov rsi, rdi
	mov rdi, 1
	int 80h
	ret

write:
	mov rdx, rsi
	mov rsi, rdi
	mov	rdi, 2
	int 80h
	ret

timeGet:
	mov rdi, 3
	int 80h
	ret
