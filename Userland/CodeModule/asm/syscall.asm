GLOBAL read
GLOBAL write
GLOBAL timeGet
GLOBAL timeSet
GLOBAL screenClear
GLOBAL intervalSet
GLOBAL processesList
GLOBAL ipcsList
GLOBAL alloc
GLOBAL processNew
GLOBAL processKill
GLOBAL stackBeyond
GLOBAL msgQueueSysCall
GLOBAL semaphoreSysCall
GLOBAL cpuSysCall
GLOBAL stackMore

read:
	mov rdx, rsi
	mov rsi, rdi
	mov rdi, 1
	int 80h
	ret

alloc:
	mov rdx, rsi
	mov rsi, rdi
	mov rdi, 7
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

processesList:
	mov rdi, 8
	int 80h
	ret

ipcsList:
	mov rdi, 9
	int 80h
	ret

intervalSet:
	mov rdx, rsi
	mov rsi, rdi
	mov rdi, 6
	int 80h
	ret

processNew:
	int 81h
	ret

msgQueueSysCall:
	int 82h
	ret

semaphoreSysCall:
	int 83h
	ret

cpuSysCall:
	int 84h
	ret

processKill:
	mov rdx, rsi
	mov rsi, rdi
	mov rdi, 10
	int 80h
	ret

stackBeyond:
	mov rdi, 23
	int 80h
	ret

stackMore:
	mov edi, 24
	int 80h
	ret