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

	movq $(0x101000), %rax
	movq %rax, %cr3
	movq lmode_entry_ptr(%rip),	%rax
	pushq $0x08
	pushq %rax
	lretq

.set stackbase, 0x7c00
.set vstackbase, 0xffff800000007c00
/*  the follwing code use new global descriptor, the loader space can be reuse */
lmode_entry:
    movq $0x10, %rax
    movq %rax, %ds
    movq %rax, %es
	movq %rax, %gs
	movq %rax, %ss

    movq $vstackbase, %rsp /* virtual address (physical memory address: 0x7c00 ) */
    
    movq start_kernel_ptr(%rip), %rax
    pushq $0x08 /* segment number */
    pushq %rax /* offset */
    lretq


lmode_entry_ptr:  .quad lmode_entry
.extern Start_Kernel
start_kernel_ptr: .quad Start_Kernel


/* ============================================================================================ */
/* ============================================================================================ */
/* ============================================================================================ */
/* ============================================================================================ */
/* ============================================================================================ */
/* ============================================================================================ */
/* ============================================================================================ */
/* ============================================================================================ */
/* the bitmask include file! */
.include "pg_mask.S"

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

GDT_Table:
	.quad 0x0000000000000000			/*0	NULL descriptor					, selector:0x00*/
	.quad 0x0020980000000000			/*1	KERNEL	Code	64-bit	Segment	, selector:0x08*/
	.quad 0x0000920000000000			/*2	KERNEL	Data	64-bit	Segment	, selector:0x10*/
	.quad 0x0000000000000000			/*3	USER	Code	32-bit	Segment , selector:0x18*/
	.quad 0x0000000000000000			/*4	USER	Data	32-bit	Segment , selector:0x20*/
	.quad 0x0020f80000000000			/*5	USER	Code	64-bit	Segment	, selector:0x28*/
	.quad 0x0000f20000000000			/*6	USER	Data	64-bit	Segment	, selector:0x30*/
	.quad 0x00cf9a000000ffff			/*7	KERNEL	Code	32-bit	Segment	, selector:0x38*/
	.quad 0x00cf92000000ffff			/*8	KERNEL	Data	32-bit	Segment	, selector:0x40*/
	.fill 10,8,0					/*10 ~ 11 TSS (jmp one segment <9>) in long-mode 128-bit 0x50*/
GDT_END:

GDT_POINTER:
GDT_LIMIT: .word GDT_END - GDT_Table - 1
GDT_BASE:  .quad GDT_Table

//=======	IDT_Table

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