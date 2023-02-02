# 0 "head.S"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 0 "<command-line>" 2
# 1 "head.S"
# 10 "head.S"
.section .text


.global _start
_start:

    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %ss

    lgdt GDT_POINTER(%rip)
    lidt IDT_POINTER(%rip)


    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %ss

    movq $0x7c00, %rsp

 movq $(0x101000), %rax
 movq %rax, %cr3

 movq lmode_entry_ptr(%rip), %rax
 pushq $0x08
 pushq %rax
 lretq

.set stackbase, 0x7c00
.set vstackbase, 0xffff800000007c00

lmode_entry:
    movq $0x10, %rax
    movq %rax, %ds
    movq %rax, %es
 movq %rax, %gs
 movq %rax, %ss
    movq $vstackbase, %rsp

fill_idt:
    leaq dummy_INThandler(%rip), %rdx

    movq %rdx, %rax
    shrq $16, %rax
    shlq $(32+16), %rax
    movw %dx, %ax
    movq $(0x8e00<<32), %rbx
    orq %rbx, %rax
    movq $(0x8<<16), %rbx
    orq %rbx, %rax

    movq %rdx, %rbx
    shrq $32, %rbx

    leaq IDT_Table(%rip), %rdi
    mov $256, %rcx
fill_idt_loop:

    movq %rax, (%rdi)
    movq %rbx, 8(%rdi)
    addq $0x10, %rdi
    loop fill_idt_loop


    movq start_kernel_ptr(%rip), %rax
    pushq $0x08
    pushq %rax
    lretq

dummy_INThandler:

 cld
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
 iretq

lmode_entry_ptr: .quad lmode_entry
.extern Start_Kernel
start_kernel_ptr: .quad Start_Kernel
# 148 "head.S"
.include "pg_mask.S"

.org 0x1000


__PML4E:

 .quad 0x102000 | PML4E_P | PML4E_RW | PML4E_US
 .fill 255,8,0

 .quad 0x102000 | PML4E_P | PML4E_RW | PML4E_US

.org 0x2000
__PDPTE:

 .quad 0x103000 | PDE_P | PDE_RW


.org 0x3000
__PDE:


 .quad 0x000000 | PDE_P | PDE_2M | PDE_RW
 .quad 0x200000 | PDE_P | PDE_2M | PDE_RW
 .quad 0x400000 | PDE_P | PDE_2M | PDE_RW
 .quad 0x600000 | PDE_P | PDE_2M | PDE_RW
 .quad 0x800000 | PDE_P | PDE_2M | PDE_RW
 .quad 0xe0000000 | PDE_P | PDE_2M | PDE_RW
 .quad 0xe0200000 | PDE_P | PDE_2M | PDE_RW
 .quad 0xe0400000 | PDE_P | PDE_2M | PDE_RW
 .quad 0xe0600000 | PDE_P | PDE_2M | PDE_RW
 .quad 0xe0800000 | PDE_P | PDE_2M | PDE_RW
 .quad 0xe0a00000 | PDE_P | PDE_2M | PDE_RW
 .quad 0xe0c00000 | PDE_P | PDE_2M | PDE_RW
 .quad 0xe0e00000 | PDE_P | PDE_2M | PDE_RW


.org 0x4000





.section .data

.globl GDT_Table

GDT_Table:
 .quad 0x0000000000000000
 .quad 0x0020980000000000
 .quad 0x0000920000000000
 .quad 0x0000000000000000
 .quad 0x0000000000000000
 .quad 0x0020f80000000000
 .quad 0x0000f20000000000
 .quad 0x00cf9a000000ffff
 .quad 0x00cf92000000ffff
 .fill 10,8,0
GDT_END:

GDT_POINTER:
GDT_LIMIT: .word GDT_END - GDT_Table - 1
GDT_BASE: .quad GDT_Table



.globl IDT_Table

IDT_Table:
 .fill 512,8,0
IDT_END:

IDT_POINTER:
IDT_LIMIT: .word IDT_END - IDT_Table - 1
IDT_BASE: .quad IDT_Table



.globl TSS64_Table

TSS64_Table:
 .fill 13,8,0
TSS64_END:


TSS64_POINTER:
TSS64_LIMIT: .word TSS64_END - TSS64_Table - 1
TSS64_BASE: .quad TSS64_Table
