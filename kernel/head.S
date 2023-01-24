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

    lgdt gdt_ptr(%rip)
    lidt idt_ptr(%rip)

    /* load the new descriptor table (the one defined in head.S) */
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %ss
    /* set the 64-bit stack pointer */
    movq $0x7c00, %rsp

	movq $(0x100000+PT_Roffset), %rax
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
    
    movq start_kernel_ptr($rip), %rax
    pushq $0x08 /* segment number */
    pushq %rax /* offset */
    lretq

/* the bitmask include file! */
.include "pg_mask.S"
/* PML4E, PDPTE, and PDE will ocupy continuous memory of size 0x1000*3 */
.org 0x1000

__PML4E:
	/*	  ADDRESS       ATTRIBUTES        */
	.quad __PDPTE | PML4E_P | PML4E_RW | PML4E_US /* VALID, WRITABLE, USER */
	.fill	255,8,0
	/* 	 ADDRESS       ATTRIBUTES        */
	.quad __PDPTE | PML4E_P | PML4E_RW | PML4E_US /* VALID, WRITABLE, USER */
	//.fill	255,8,0

.org 0x2000

__PDPTE:
	/*    ADDRESS    ATTRIBUTES        */
	.quad __PDE | PDE_P | PDE_RW /* VALID, WRITABLE, SYSTEM	 */
	//.fill	511,8,0

.org 0x3000

__PDE:
	/* VALID, 2M PAGES, WRITABLE, SYSTEM */
	/*     ADDRESS            ATTRIBUTES        */
	.quad 0x000000 		| PDE_P | PDE_2M | PDE_RW	
	.quad 0x200000 		| PDE_P | PDE_2M | PDE_RW
	.quad 0x400000 		| PDE_P | PDE_2M | PDE_RW
	.quad 0x600000		| PDE_P | PDE_2M | PDE_RW
	.quad 0x800000	 	| PDE_P | PDE_2M | PDE_RW
	.quad 0xe0000000 	| PDE_P | PDE_2M | PDE_RW		/*0xa00000*/
	.quad 0xe0200000 	| PDE_P | PDE_2M | PDE_RW
	.quad 0xe0400000 	| PDE_P | PDE_2M | PDE_RW
	.quad 0xe0600000 	| PDE_P | PDE_2M | PDE_RW		/*0x1000000*/
	.quad 0xe0800000 	| PDE_P | PDE_2M | PDE_RW
	.quad 0xe0a00000 	| PDE_P | PDE_2M | PDE_RW
	.quad 0xe0c00000 	| PDE_P | PDE_2M | PDE_RW
	.quad 0xe0e00000 	| PDE_P | PDE_2M | PDE_RW
	//.fill	499,8,0
.org 0x4000 /* fill in the spare memory from offset 0x3000 ~ 0x4000 with zero */

.section .data
lmode_entry_ptr:    .quad lmode_entry
start_kernel_ptr:   .quad start_kernel