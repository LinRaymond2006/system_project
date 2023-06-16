# 0 "asm_interface.S"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 0 "<command-line>" 2
# 1 "asm_interface.S"





.global switch_context
.extern cur_regstruct_ptr
.extern Schedule

.set regstruct_GS, 0x0
.set regstruct_FS, 0x8
.set regstruct_ES, 0x10
.set regstruct_DS, 0x18
.set regstruct_R15, 0x20
.set regstruct_R14, 0x28
.set regstruct_R13, 0x30
.set regstruct_R12, 0x38
.set regstruct_R11, 0x40
.set regstruct_R10, 0x48
.set regstruct_R9, 0x50
.set regstruct_R8, 0x58
.set regstruct_RBP, 0x60
.set regstruct_RDI, 0x68
.set regstruct_RSI, 0x70
.set regstruct_RDX, 0x78
.set regstruct_RCX, 0x80
.set regstruct_RBX, 0x88
.set regstruct_RAX, 0x90
.set regstruct_RIP, 0x98
.set regstruct_CS, 0xa0
.set regstruct_RFLAGS, 0xa8
.set regstruct_RSP, 0xb0
.set regstruct_SS, 0xb8


switch_context:

 pushq $0
    xchgq (%rsp), %rax
 movq (cur_regstruct_ptr), %rax






 xchg %rax, %rsp
# 61 "asm_interface.S"
 pushq 0x30(%rax)
 pushq 0x28(%rax)
 pushq 0x20(%rax)
 pushq 0x18(%rax)
 pushq 0x10(%rax)
 pushq (%rax)



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

 movq %ds, %rbx
 pushq %rbx

 movq %es, %rbx
 pushq %rbx





 movq %fs, %rbx
 pushq %rbx

 movq %gs, %rbx
 pushq %rbx

 xchgq %rax, %rsp


 movq $0x10, %rdi
 movq %rdi, %ds
 movq %rdi, %es


 leaq (Schedule), %rdx
 callq *%rdx







 movq regstruct_SS(%rax), %rbx
 movq %rbx, 0x30(%rsp)
 movq regstruct_RSP(%rax), %rbx
 movq %rbx, 0x28(%rsp)
 movq regstruct_RFLAGS(%rax), %rbx
 movq %rbx, 0x20(%rsp)
 movq regstruct_CS(%rax), %rbx
 movq %rbx, 0x18(%rsp)
 movq regstruct_CS(%rax), %rbx
 movq %rbx, 0x10(%rsp)

 xchgq %rax, %rsp



 popq %rbx
 movq %rbx, %gs

 popq %rbx
 movq %rbx, %fs


 popq %rbx
 movq %rbx, %es


 popq %rbx
 movq %rbx, %ds

 popq %r15
 popq %r14
 popq %r13
 popq %r12
 popq %r11
 popq %r10
 popq %r9
 popq %r8
 popq %rbp
 popq %rdi
 popq %rsi
 popq %rdx
 popq %rcx
 popq %rbx
 xchgq %rax, %rsp
 movq (%rax), %rax


 iretq
