.text
# Save registers
	.globl	jkthread_save_regs
	.type	jkthread_save_regs,	@function
jkthread_save_regs:
	# Save registers
	movq	%rbx,	0(%rdi)
	movq	%rsp,	8(%rdi)
	movq	%rbp,	16(%rdi)
	movq	%r12,	24(%rdi)
	movq	%r13,	32(%rdi)
	movq	%r14,	40(%rdi)
	movq	%r15,	48(%rdi)

	ret

# Restore registers
	.size	jkthread_save_regs,	.-jkthread_save_regs
	.globl	jkthread_restore_regs
	.type	jkthread_restore_regs,	@function
jkthread_restore_regs:
	# Restore registers of thread being switched to
	movq	0(%rdi),	%rbx
	movq	8(%rdi),	%rsp
	movq	16(%rdi),	%rbp
	movq	24(%rdi),	%r12
	movq	32(%rdi),	%r13
	movq	40(%rdi),	%r14
	movq	48(%rdi),	%r15

	# Save calling thread's RIP value such that it skips the next jmp
	# upon restoration
	leaq	.L1(%rip),	%rax
	movq	%rax,	(%rsi)

	# Save RIP of thread being switched to
	movq	56(%rdi),	%rax
	# Check if RDX is NULL, and if so, skip input parameter copy
	cmpq	$0,	%rdx
	je	.L0
	# Copy input parameter to RDI
	movq	(%rdx),	%rdi
.L0:
	# Set RIP of thread being switched to
	jmp	*%rax

	# Return once this function is context switched back to
.L1:	ret
	.size	jkthread_restore_regs,	.-jkthread_restore_regs
