	.file	"main.c"
	.intel_syntax noprefix
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	mov	DWORD PTR [rbp-36], edi
	mov	QWORD PTR [rbp-48], rsi
	mov	DWORD PTR [rbp-32], 0
	mov	BYTE PTR [rbp-28], 0
	movzx	eax, BYTE PTR [rbp-32]
	mov	BYTE PTR [rbp-16], al
	mov	eax, DWORD PTR [rbp-31]
	mov	DWORD PTR [rbp-12], eax
	movzx	eax, BYTE PTR [rbp-16]
	movsx	edx, al
	mov	eax, DWORD PTR [rbp-12]
	add	eax, edx
	pop	rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (GNU) 5.1.1 20150618 (Red Hat 5.1.1-4)"
	.section	.note.GNU-stack,"",@progbits
