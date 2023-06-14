#ifndef __REGSTRUCT_H__
#define __REGSTRUCT_H__

/*
struct memory layout, for example:
struct {
	//lower part of memory
	int a;	//offset: 0
	int b;	//offset: 4
	int c;	//offset: 8
	//higher part of memory
}




*/



//the struct IntRegisterSet isn't tested, not sure whether it's correct or not
/*


	//write the handler address into stack
	xchgq %rax, (%rsp) //the error handler is filled with correct value (by caller, using leaq instruction) , and the old rax is back!

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

struct IntRegisterSet {
	unsigned long es;
	unsigned long ds;
	unsigned long r15;
	unsigned long r14;
	unsigned long r13;
	unsigned long r12;
	unsigned long r11;
	unsigned long r10;
	unsigned long r9;
	unsigned long r8;
	unsigned long rbp;
	unsigned long rdi;
	unsigned long rsi;
	unsigned long rdx;
	unsigned long rcx;
	unsigned long rbx;
	unsigned long rax;
	unsigned long handler;
	unsigned long errorcode;
	unsigned long rip;
	unsigned long cs;
	unsigned long rflags;
	unsigned long rsp;
	unsigned long ss;
} __attribute__ ((packed));



struct RegisterSet {
	//low memory
    unsigned long gs;
    unsigned long fs;
	unsigned long es;
	unsigned long ds;
	unsigned long r15;
	unsigned long r14;
	unsigned long r13;
	unsigned long r12;
	unsigned long r11;
	unsigned long r10;
	unsigned long r9;
	unsigned long r8;
	unsigned long rbp;
	unsigned long rdi;
	unsigned long rsi;
	unsigned long rdx;
	unsigned long rcx;
	unsigned long rbx;
	unsigned long rax;
	unsigned long rip;
	unsigned long cs;
	unsigned long rflags;
	unsigned long rsp;
	unsigned long ss;
	//high memory
} __attribute__ ((packed));




typedef struct Register_set ctxt_regs;

#endif