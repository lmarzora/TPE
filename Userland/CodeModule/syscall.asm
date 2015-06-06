GLOBAL read
GLOBAL write
GLOBAL timeGet
GLOBAL timeSet
GLOBAL screenClear
read:
	mov rdx, rsi
	mov rsi, rdi
	mov rdi, 1
	int 80h
	ret

write:
	mov rdx, rsi
	mov rsi, rdi
	mov rdi, 2
	int 80h
	ret

timeGet:
	mov rdx, rsi
	mov rsi, rdi
	mov rdi, 3
	int 80h
	ret

timeSet:
	mov rdx, rsi
	mov rsi ,rdi
	mov rdi, 4
	int 80h
	ret

screenClear:
	mov rdi, 5
	int 80h
	ret
