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

.align 8 /* note: the page table have to be 8-byte aligned */
.set PT_Roffset, 0x1000        /* relative offset of the whole stucture of page table will be stored at memory 0x100000+0x1000*/
.set PML4E_Roffset, PT_Roffset /* relative offset of PML4E */
.set PML4E_sz, 0x1000
.set PDPTE_Roffset, (PT_Roffset+PML4E_sz) /* relative offset of PML4E */
.set PDPTE_sz, 0x1000
.set PDE_Roffset, (PDPTE_Roffset+PDPTE_sz) /* relative offset of PDE */
.set PDE_sz, 0x1000

/* the bitmask include file! */
.include "pg_mask.S"

/* PML4E, PDPTE, and PDE will ocupy continuous memory of size 0x1000*3 */
.org PML4E_Roffset

__PML4E:

	.quad	0x102007
	.fill	255,8,0
	.quad	0x102007
	//.fill	255,8,0

.org PDPTE_Roffset

__PDPTE:
	
	.quad	0x103003
	//.fill	511,8,0

.org PDE_Roffset

__PDE:

	.quad	0x000083	
	.quad	0x200083
	.quad	0x400083
	.quad	0x600083
	.quad	0x800083
	.quad	0xe0000083		/*0x a00000*/
	.quad	0xe0200083
	.quad	0xe0400083
	.quad	0xe0600083		/*0x1000000*/
	.quad	0xe0800083
	.quad	0xe0a00083
	.quad	0xe0c00083
	.quad	0xe0e00083
	//.fill	499,8,0
.org PDE_Roffset+PDE_sz /* fill in the spare memory from offset 0x3000 ~ 0x4000 with zero */

.section .data
lmode_entry_ptr:    .quad lmode_entry
start_kernel_ptr:   .quad start_kernel