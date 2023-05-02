/*
    the assembly part in kernel will be writen in GNU assembler format
    note: the program use gcc as C preprocessor to expand macros
    code in "head.S" are mostly cloned from book <<一个64位操作系统的设计与实现>> (isbn 9787115475251)
*/

/* ===========text=section=========== */
/* note: the code below are 64-bits instructions */

.section .text

/* the linker will identify the _start label as entry point */
.global _start
_start:
    /* only cs are flushed by the far jump, so other sregs need to be set */
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %ss

    lgdt GDT_POINTER(%rip)
    lidt IDT_POINTER(%rip)

    /* load the new descriptor table (the one defined in head.S) */
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %ss
    /* set the 64-bit stack pointer */
    movq $0x7c00, %rsp
	/* load the new 4-level paging table */
	movq $(0x101000), %rax
	movq %rax, %cr3

	movq lmode_entry_ptr(%rip),	%rax
	pushq $0x08
	pushq %rax
	lretq

/* ============================================================================================ */
/* ============================================================================================ */
/* ============================================================================================ */
.set stackbase, 0x7c00
.set vstackbase, 0xffff800000000000+stackbase
/*  the follwing code use new global descriptor, the loader space can be reuse */
lmode_entry:
	cli
    movq $0x10, %rax
    movq %rax, %ds
    movq %rax, %es
	movq %rax, %gs
	movq %rax, %ss
    movq $vstackbase, %rsp /* virtual address (physical memory address: 0x7c00 ) */

fill_idt:
    leaq dummy_INThandler(%rip), %rdx /* rdx hold the address of the handler function now */
	/* the lower part (bit 0 ~ bit 63) of IDT stay in RAX register */
    movq %rdx, %rax
    shrq $16, %rax
    shlq $(32+16), %rax		/* clear the unused bits to zero by shifting right and left */
    movw %dx, %ax			/* IDT bit 0 ~ bit 15 (offset bit 0~15) */
    movq $(0x8e00<<32), %rbx	/* attribute field */
    orq %rbx, %rax
    movq $(0x8<<16), %rbx /* the kernel code segment descriptor (#1 descriptor, 0x8) */
    orq %rbx, %rax
	/* the higher part (bit 64 ~ bit 127) of IDT stay in RBX register */
    movq %rdx, %rbx
    shrq $32, %rbx
	/* the pointer of "IDT_Table" will fill in each entry */
    leaq    IDT_Table(%rip),    %rdi    /*rdi=IDT_Table*/
    mov    $256,    %rcx            /* rcx=256 (times) */
fill_idt_loop:
	/* since there no specific inturrupt handler now, every inturrupt descriptor table entry points to the same handler function "dummy_INThandler" */
    movq    %rax,    (%rdi)     /* *(rdi) =rax   */
    movq    %rbx,    8(%rdi)	/* *(rdi+8)=rdx   */
    addq    $0x10,    %rdi 		/* rdi+=16(0x10) --> set pointer to next entry */
    loop fill_idt_loop

	/* please note that TSS are not initialized yet, plus there no real error handler, the system is extremely vulnerable now! */

.include "def/desc_mask.S"

setup_TSS64:
	leaq	TSS64_Table(%rip),	%rdx

	xorq	%rcx,	%rcx
	movq	$0x89,	%rax	//VALID_TSS | DESC_TSS | TSS_DPL_SYS | AVAILABLE_TSS64
	shlq	$40,	%rax
	movl	%edx,	%ecx
	shrl	$24,	%ecx
	shlq	$56,	%rcx
	addq	%rcx,	%rax
	xorq	%rcx,	%rcx
	movl	%edx,	%ecx
	andl	$0xffffff,	%ecx
	shlq	$16,	%rcx
	addq	%rcx,	%rax
	addq	$103,	%rax	//TSS64_LIMIT
	leaq	GDT_Table(%rip),	%rdi
	movq	%rax,	0x50(%rdi)
	shrq	$32,	%rdx
	movq	%rdx,	0x58(%rdi)

	mov	$0x50,	%ax
	ltr	%ax

	/* perform a far jump and go to the main part of the kernel, which is implemnented in C */
    movq start_kernel_ptr(%rip), %rax
    pushq $0x08 /* segment number */
    pushq %rax /* offset */

	sti

    lretq


/* ============================================================================================ */
/* ============================================================================================ */
/* ============================================================================================ */
/* some function below: */
dummy_INThandler:
	/* the pushad/popad is not immplement in IA32e arch so the function has to use a lot of push / pop instruction instead*/
	cli
	/* save the value */
	pushq %rax
	pushq %rbx
	pushq %rcx
	pushq %rdx
	pushq %rbp
	pushq %rdi
	pushq %rsi
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
	movq %es, %rax
	pushq %rax
	movq %ds, %rax
	pushq %rax
	movq $0x10, %rax
	movq %rax, %ds
	movq %rax, %es

	/* the inturrupt do nothing, return to the caller with register value unchanged */
	/* execute a "nop" instruction maybe??! */
	nop

	/* restore the value */
	popq %rax
	movq %rax, %ds
	popq %rax
	movq %rax, %es
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rsi
	popq %rdi
	popq %rbp
	popq %rdx
	popq %rcx
	popq %rbx
	popq %rax
	sti
	
	label_halt:
		nop
		nop
		nop
		nop
		jmp label_halt

	iretq

/* ============================================================================================ */
/* ============================================================================================ */
/* ============================================================================================ */
/* some function pointer below: */
.extern Start_Kernel
lmode_entry_ptr:  .quad lmode_entry
start_kernel_ptr: .quad Start_Kernel

tss_lower_mask:
	.quad VALID_TSS | DESC_TSS | TSS_DPL_SYS | AVAILABLE_TSS64
	/* ============================================================================================ */
/* ============================================================================================ */
/* ============================================================================================ */
/* ============================================================================================ */
/* ============================================================================================ */
/* the bitmask include file! */
.include "def/pg_mask.S"

.org 0x1000
/* PML4E, PDPTE, and PDE will ocupy continuous memory of size 0x1000*3 */

__PML4E:
	/*	  ADDRESS       ATTRIBUTES        */
	.quad 0x102000 | PML4E_P | PML4E_RW | PML4E_US /* VALID, WRITABLE, USER */
	.fill	255,8,0
	/* 	 ADDRESS       ATTRIBUTES        */
	.quad 0x102000 | PML4E_P | PML4E_RW | PML4E_US /* VALID, WRITABLE, USER */

.org 0x2000
__PDPTE:
	/*    ADDRESS    ATTRIBUTES        */
	.quad 0x103000 | PDE_P | PDE_RW /* VALID, WRITABLE, SYSTEM	 */


.org 0x3000
__PDE:
	/* VALID, 2M PAGES, WRITABLE, SYSTEM */
	/*     ADDRESS            ATTRIBUTES        */
	.quad 0x000000 		| PDE_P | PDE_2M | PDE_RW	
	.quad 0x200000 		| PDE_P | PDE_2M | PDE_RW
	.quad 0x400000 		| PDE_P | PDE_2M | PDE_RW
	.quad 0x600000		| PDE_P | PDE_2M | PDE_RW
	.quad 0x800000	 	| PDE_P | PDE_2M | PDE_RW
	.quad 0xe0000000 	| PDE_P | PDE_2M | PDE_RW		/* 0xa00000 */
	.quad 0xe0200000 	| PDE_P | PDE_2M | PDE_RW
	.quad 0xe0400000 	| PDE_P | PDE_2M | PDE_RW
	.quad 0xe0600000 	| PDE_P | PDE_2M | PDE_RW		/* 0x1000000 */
	.quad 0xe0800000 	| PDE_P | PDE_2M | PDE_RW
	.quad 0xe0a00000 	| PDE_P | PDE_2M | PDE_RW
	.quad 0xe0c00000 	| PDE_P | PDE_2M | PDE_RW
	.quad 0xe0e00000 	| PDE_P | PDE_2M | PDE_RW


.org 0x4000 /* fill in the spare memory from offset 0x3000 ~ 0x4000 with zero */
/* ============================================================================================ */
/* ============================================================================================ */
/* ============================================================================================ */
/* ============================================================================================ */
/* ============================================================================================ */
.section .data

.globl GDT_Table


//use 4-level paging to manage the memory, minimum descriptor are required (data/code for each privillage)
GDT_Table:
	.quad NULL_DESC
	//      ++++++++----++++--------++++++++----------------++++++++++++++++
	//.quad 0b0000000000100000100110000000000000000000000000000000000000000000			/* 1: 64 bit kernel code, selector 0x08 */
	.quad VALID_SEGDESC | DESC_GDT | SEG_64BIT | SEGDESC_DPL_SYS | EXEC | SEG_CODE_NONCONFIRMED

	//      ++++++++----++++--------++++++++----------------++++++++++++++++
	.quad 0b0000000000000000100100100000000000000000000000000000000000000000			/* 2: 64 bit kernel data, selector 0x10 */
	.quad NULL_DESC			/* 3: 32 bit user code, selector 0x18 */
	.quad NULL_DESC			/* 4: 32 bit user data, selector 0x20 */
	//      ++++++++----++++--------++++++++----------------++++++++++++++++
 	.quad 0b0000000000100000111110000000000000000000000000000000000000000000			/* 5: 64 bit user code, selector 0x28 */
	//      ++++++++----++++--------++++++++----------------++++++++++++++++
	.quad 0b0000000000000000111100100000000000000000000000000000000000000000			/* 6: 64 bit user data, selector 0x30 */
	//      ++++++++----++++--------++++++++----------------++++++++++++++++
	.quad 0b0000000011001111100110100000000000000000000000001111111111111111			/* 7: 32 bit kernel code, selector 0x38 */
	//      ++++++++----++++--------++++++++----------------++++++++++++++++
	.quad 0b0000000011001111100100100000000000000000000000001111111111111111			/* 8: 32 bit kernel data, selector 0x40 */
	.fill 10,8,NULL_DESC						/* 10.11 TSS descriptor, selector 0x50 */
GDT_END:

GDT_POINTER:
GDT_LIMIT: .word GDT_END - GDT_Table - 1
GDT_BASE:  .quad GDT_Table

//=======	IDT_Table

.globl IDT_POINTER
.globl IDT_Table

IDT_Table:
	.fill 512,8,0
IDT_END:

IDT_POINTER:
IDT_LIMIT: .word IDT_END - IDT_Table - 1
IDT_BASE:  .quad IDT_Table

//=======	TSS64_Table

.globl	TSS64_Table

TSS64_Table:
	.fill 13,8,0
TSS64_END:


TSS64_POINTER:
TSS64_LIMIT: .word TSS64_END - TSS64_Table - 1
TSS64_BASE:  .quad TSS64_Table
/* ============================================================================================ */
/* ============================================================================================ */
/* ============================================================================================ */
/* ============================================================================================ */