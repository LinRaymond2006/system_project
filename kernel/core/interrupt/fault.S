#include "linkage.h"
#define REG_R15 0x00
#define REG_R14 0x08
#define REG_R13 0x10
#define REG_R12 0x18
#define REG_R11 0x20
#define REG_R10 0x28
#define REG_R9 0x30
#define REG_R8 0x38
#define REG_RBX 0x40
#define REG_RCX 0x48
#define REG_RDX 0x50
#define REG_RSI 0x58
#define REG_RDI 0x60
#define REG_RBP 0x68
#define SREG_DS 0x70
#define SREG_ES 0x78
#define REG_RAX 0x80
#define HANDLER_ADDR 0x88
#define ERRCODE 0x90
#define REG_RIP 0x98
#define SREG_CS 0xa0
#define CREG_RFLAGS 0xa8
#define REG_RSP_OLD 0xb0
#define SREG_SS_OLD 0xb8
IsrPrologue:
    //pushq %rax
	pushq	%rax
    //pushq %es
	movq	%es,	%rax
	pushq	%rax
    //pushq %ds
	movq	%ds,	%rax
	pushq	%rax
    //push other registers
	pushq	%rbp
	pushq	%rdi
	pushq	%rsi
	pushq	%rdx
	pushq	%rcx
	pushq	%rbx
	pushq	%r8
	pushq	%r9
	pushq	%r10
	pushq	%r11
	pushq	%r12
	pushq	%r13
	pushq	%r14
	pushq	%r15	
	
	cld
	movq	ERRCODE(%rsp),	%rsi
	movq	HANDLER_ADDR(%rsp),	%rdx	
    
    //kernel 64 bit data selector
	movq	$0x10,	%rdi
	movq	%rdi,	%ds
	movq	%rdi,	%es

    //save the old rsp into rdi
	movq	%rsp,	%rdi
    //invoke handler function
	callq 	*%rdx

	jmp	exit_isr	

IsrEpilogue:
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rbx
	popq %rcx
	popq %rdx
	popq %rsi
	popq %rdi
	popq %rbp
    //popq %ds
	popq %rax
	movq %rax, %ds
    //popq %es
	popq %rax
	movq %rax, %es
    //popq %rax
	popq %rax
	addq $0x10, %rsp
	iretq





registeryDE:
	pushq	$0          //error code
	pushq	%rax        //save old rax value
    //use rax as function pointer
	leaq	do_divide_error(%rip),	%rax
    //set program
	xchgq	%rax,	(%rsp)
    jmp IsrPrologue