# 1 "head.S"
# 1 "<built-in>"
# 1 "<command-line>"
# 31 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 32 "<command-line>" 2
# 1 "head.S"






.section .text


.global _start

_start:

    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %ss

    lgdt gdt_ptr(%rip)
    lidt idt_ptr(%rip)


    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %ss

    movq $0x7c00, %rsp

 movq $0x101000, %rax
 movq %rax, %cr3
 movq goto_64(%rip), %rax
 pushq $0x08
 pushq %rax
 lretq

goto_64:
    .quad lmode_entry


lmode_entry:
    movq $0x10, %rax
    movq %rax, %ds
    movq %rax, %es
 movq %rax, %gs
 movq %rax, %ss

.section .data
