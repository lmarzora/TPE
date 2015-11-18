GLOBAL cpuVendor
GLOBAL getCR3
GLOBAL setCR3

section .text
	
cpuVendor:
	push rbp
	mov rbp, rsp

	push rbx

	mov rax, 0
	cpuid


	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx

	mov byte [rdi+13], 0

	mov rax, rdi

	pop rbx

	mov rsp, rbp
	pop rbp
	ret

getCR3:
	mov rax, cr3
	ret
setCR3:
	mov cr3, rdi
	ret

