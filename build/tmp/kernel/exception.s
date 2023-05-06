# 0 "fault.S"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 0 "<command-line>" 2
# 1 "fault.S"
# 26 "fault.S"
.globl divide_error

divide_error:
 pushq $0
 pushq %rax
 leaq do_divide_error(%rip), %rax
 xchgq %rax, (%rsp)
 jmp IsrLauncher

IsrLauncher:

 pushq %rax

 movq %es, %rax
 pushq %rax

 movq %ds, %rax
 pushq %rax

 pushq %rbp
 pushq %rdi
 pushq %rsi
 pushq %rdx
 pushq %rcx
 pushq %rbx
 pushq %r8
 pushq %r9
 pushq %r10
 pushq %r11
 pushq %r12
 pushq %r13
 pushq %r14
 pushq %r15

 cld
 movq 0x90(%rsp), %rsi

 movq $0x10, %rdi
 movq %rdi, %ds
 movq %rdi, %es

 movq %rsp, %rdi

 movq 0x88(%rsp), %rdx

 callq *%rdx

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

 popq %rax
 movq %rax, %ds

 popq %rax
 movq %rax, %es

 popq %rax

 addq $(0x8+0x8), %rsp
 iretq
