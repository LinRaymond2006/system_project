//set by host system--------
.set SREG_ES, 0x00
.set SREG_DS, 0x08
.set REG_R15, 0x10
.set REG_R14, 0x18
.set REG_R13, 0x20
.set REG_R12, 0x28
.set REG_R11, 0x30
.set REG_R10, 0x38
.set REG_R9, 0x40
.set REG_R8, 0x48
.set REG_RBP, 0x50
.set REG_RDI, 0x58
.set REG_RSI, 0x60
.set REG_RDX, 0x68
.set REG_RCX, 0x70
.set REG_RBX, 0x78
.set REG_RAX, 0x80
.set HANDLER_ADDR, 0x88
//--------------------------

//set by processor----------
.set ERRCODE, 0x90
.set REG_RIP, 0x98
.set SREG_CS, 0xa0
.set CREG_RFLAGS, 0xa8
.set REG_RSP_OLD, 0xb1
.set SREG_SS_OLD, 0xb8
//--------------------------
.globl Isr_8259A_General
.extern PicHandlerGeneral
Isr_8259A_General:
    pushq $0        //dummy error code (padding), pass the #NR to the program since error code field is not used. NOTE: push constant, do not ultilize any register else
    pushq %rax      //dummy handler (padding), store the old rax
    leaq PicHandlerGeneral(%rip), %rax 
    xchgq %rax, (%rsp) //the error handler is filled with correct value, and the old rax is back!
    jmp PIC8259A_IsrLauncher

PIC8259A_IsrLauncher:
	cld

    pushq %rax      //real eax
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
	
	//switch to kernel 64 bit data segment
	movq $0x10, %rdi
	movq %rdi, %ds
	movq %rdi, %es


	//system V calling convention
	movq %rsp, %rdi             //register set pointer
    movq ERRCODE(%rsp), %rsi    //#NR value
	movq HANDLER_ADDR(%rsp), %rdx
	

	callq *%rdx

	popq %rax
	popq %rbx
	popq %rcx
	popq %rdx
	popq %rsi
	popq %rdi
	popq %rbp
	popq %r8
	popq %r9
	popq %r10
	popq %r11
	popq %r12
	popq %r13
	popq %r14
	popq %r15
	//popq %ds is invalid
	popq %rax
	movq %rax, %ds
	//popq %es is invalid
	popq %rax
	movq %rax, %es


	//balance the stack (error code, handler)
	//other stuff are maintain by the processor
	addq $(0x8+0x8), %rsp
	iretq