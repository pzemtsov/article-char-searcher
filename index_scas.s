	.text
	.p2align 5,,31
	.globl	index_scas
	.type	index_scas, @function

# const void* index_scas(const void* ptr, int ch, size_t count)
# RDI = ptr
# RSI = ch
# RDX = count

index_scas:
	.cfi_startproc
	mov     %esi, %eax
        mov     %rdx, %rcx
        repne scasb
        jnz     not_found
        lea     -1(%rdi), %rax
        ret
not_found:
        xorq    %rax, %rax
        ret
	.cfi_endproc
	.section	.note.GNU-stack,"",@progbits
