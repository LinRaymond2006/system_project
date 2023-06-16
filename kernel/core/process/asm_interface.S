//save registers in pt_regs structure of the current context (pcb structure)
//...
//load the new register set of the next context
//iretq to jump to the new context

.global switch_context
.extern cur_regstruct_ptr
.extern Schedule

.set regstruct_GS, 0x0
.set regstruct_FS, 0x8
.set regstruct_ES, 0x10
.set regstruct_DS, 0x18
.set regstruct_R15, 0x20
.set regstruct_R14, 0x28
.set regstruct_R13, 0x30
.set regstruct_R12, 0x38
.set regstruct_R11, 0x40
.set regstruct_R10, 0x48
.set regstruct_R9, 0x50
.set regstruct_R8, 0x58
.set regstruct_RBP, 0x60
.set regstruct_RDI, 0x68
.set regstruct_RSI, 0x70
.set regstruct_RDX, 0x78
.set regstruct_RCX, 0x80
.set regstruct_RBX, 0x88
.set regstruct_RAX, 0x90
.set regstruct_RIP, 0x98
.set regstruct_CS, 0xa0
.set regstruct_RFLAGS, 0xa8
.set regstruct_RSP, 0xb0
.set regstruct_SS, 0xb8


switch_context:
	//not sure whether to fill in the errorcode or not, if error code is needed/not needed, restore/comment the next line
	pushq $0
    xchgq (%rsp), %rax
	movq (cur_regstruct_ptr), %rax
	//store SS
	//store rsp
	//store rflags
	//store cs
	//store rip
	//store rax, not sure how.
	xchg %rax, %rsp	
	//rsp is now the pointer to register struct
	//rax points to the the interrupt stack

	//firstly, retrieve the information on the stack saved by processor!

	//when rsp points to handler_addr
	//rsp+0x8=errcode
	//rsp+0x10=rip
	//rsp+0x18=cs
	//rsp+0x20=rflags
	//rsp+0x28=old_rsp
	//rsp+0x30=ss

	pushq 0x30(%rax)	//ss
	pushq 0x28(%rax)	//rsp
	pushq 0x20(%rax)	//rflags
	pushq 0x18(%rax)	//cs
	pushq 0x10(%rax)	//rip
	pushq (%rax)		//rax

	//rax is still holding the interrupt stack rsp, do not overwrite it!

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
	//pushq %ds
	movq %ds, %rbx
	pushq %rbx
	//pushq %es
	movq %es, %rbx
	pushq %rbx

	//gs and fs are accessed throughout special MSR register

	//remember to MSRs for fs and gs
	//fs
	movq %fs, %rbx
	pushq %rbx
	//gs
	movq %gs, %rbx
	pushq %rbx

	xchgq %rax, %rsp	//switch the interrupt stack back

	//switch to kernel 64 bit data segment
	movq $0x10, %rdi
	movq %rdi, %ds
	movq %rdi, %es

	//determine which context to switch to
	leaq (Schedule), %rdx
	callq *%rdx

	//rax (return value) will hold the pointer of new context register set
	//switch to the context!

	//restore the registers (update the pointers in the interrupt stack first!)
	//NOT TESTED!

	movq regstruct_SS(%rax), %rbx
	movq %rbx, 0x30(%rsp)
	movq regstruct_RSP(%rax), %rbx
	movq %rbx, 0x28(%rsp)
	movq regstruct_RFLAGS(%rax), %rbx
	movq %rbx, 0x20(%rsp)
	movq regstruct_CS(%rax), %rbx
	movq %rbx, 0x18(%rsp)
	movq regstruct_CS(%rax), %rbx
	movq %rbx, 0x10(%rsp)

	xchgq %rax, %rsp
	//remember the stack is switched

	//gs
	popq %rbx
	movq %rbx, %gs
	//fs
	popq %rbx
	movq %rbx, %fs

	//es
	popq %rbx
	movq %rbx, %es

	//ds
	popq %rbx
	movq %rbx, %ds

	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rbp
	popq %rdi
	popq %rsi
	popq %rdx
	popq %rcx
	popq %rbx
	xchgq %rax, %rsp
	movq (%rax), %rax

	//execute the process!
	iretq