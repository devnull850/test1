	.section .text
	movq	$0xa21646c726f,%rax
	pushq	%rax
	movq	$0x57202c6f6c6c6548,%rax
	pushq	%rax
	xorl	%edi,%edi
	pushq	%rdi
	inc	%edi
	movl	%edi,%eax
	leaq	0x8(%rsp),%rsi
	leaq	0xd(%rdi),%rdx
	syscall
	popq	%rdi
	leaq	0x2e(%rax),%rax
	jmp	.text+0x26
