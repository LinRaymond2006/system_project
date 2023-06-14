//save registers in pt_regs structure of the current context (pcb structure)
//...
//load the new register set of the next context
//iretq to jump to the new context

.global switch_context
.extern cur_regstruct_ptr

switch_context:
	//not sure whether to fill in the errorcode or not, if error code is needed, restore the next line
	//pushq $0
    xchgq (%rsp), %rax
	movq (cur_regstruct_ptr), %rax
/*
	pushq %rax
	pushq %rbx
	pushq %rcx
	pushq %rdx
	pushq %rsi
	pushq %rdi
	pushq %rbp
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
	//pushq %ds is invalid
	movq %ds, %rax
	pushq %rax
	//pushq %es is invalid
	movq %es, %rax
	pushq %rax
*/
    leaq (  )