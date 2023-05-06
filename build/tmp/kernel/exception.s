# 0 "fault.S"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 0 "<command-line>" 2
# 1 "fault.S"
# 28 "fault.S"
IsrLauncher:
 xchgq %rax, (%rsp)


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




.globl Isr_DE
.extern DE_handler
Isr_DE:
 pushq $0
 pushq %rax
 leaq DE_handler(%rip), %rax
 jmp IsrLauncher

.globl Isr_DB
.extern DB_handler
Isr_DB:
 pushq $0
 pushq %rax
 leaq DB_handler(%rip), %rax
 jmp IsrLauncher



.globl Isr_NMI
.extern NMI_handler
Isr_NMI:
 pushq %rax
 leaq NMI_handler(%rip), %rax
 jmp IsrLauncher

.globl Isr_BP
.extern BP_handler
Isr_BP:
 pushq $0
 pushq %rax
 leaq BP_handler(%rip), %rax
 jmp IsrLauncher


.globl Isr_OF
.extern OF_handler
Isr_OF:
 pushq $0
 pushq %rax
 leaq OF_handler(%rip), %rax
 jmp IsrLauncher

.globl Isr_BR
.extern BR_handler
Isr_BR:
 pushq $0
 pushq %rax
 leaq BR_handler(%rip), %rax
 jmp IsrLauncher

.globl Isr_UD
.extern UD_handler
Isr_UD:
 pushq %rax
 leaq UD_handler(%rip), %rax
 jmp IsrLauncher

.globl Isr_NM
.extern NM_handler
Isr_NM:
 pushq $0
 pushq %rax
 leaq NM_handler(%rip), %rax
 jmp IsrLauncher

.globl Isr_DF
.extern DF_handler
Isr_DF:
 pushq %rax
 leaq DF_handler(%rip), %rax
 jmp IsrLauncher

.globl Isr_FPU_SEG_ERR
.extern FPU_SEG_ERR_handler
Isr_FPU_SEG_ERR:
 pushq $0
 pushq %rax
 leaq FPU_SEG_ERR_handler(%rip), %rax
 jmp IsrLauncher

.globl Isr_TS
.extern TS_handler
Isr_TS:
 pushq %rax
 leaq TS_handler(%rip), %rax
 jmp IsrLauncher

.globl Isr_NP
.extern NP_handler
Isr_NP:
 pushq %rax
 leaq NP_handler(%rip), %rax
 jmp IsrLauncher

.globl Isr_SS
.extern SS_handler
Isr_SS:
 pushq %rax
 leaq SS_handler(%rip), %rax
 jmp IsrLauncher

.globl Isr_GP
.extern GP_handler
Isr_GP:
 pushq %rax
 leaq GP_handler(%rip), %rax
 jmp IsrLauncher

.globl Isr_PF
.extern PF_handler
Isr_PF:
 pushq %rax
 leaq PF_handler(%rip), %rax
 jmp IsrLauncher

.globl Isr_MF
.extern MF_handler
Isr_MF:
 pushq $0
 pushq %rax
 leaq MF_handler(%rip), %rax
 jmp IsrLauncher

.globl Isr_AC
.extern AC_handler
Isr_AC:
 pushq %rax
 leaq AC_handler(%rip), %rax
 jmp IsrLauncher

.globl Isr_MC
.extern MC_handler
Isr_MC:
 pushq $0
 pushq %rax
 leaq MC_handler(%rip), %rax
 jmp IsrLauncher

.globl Isr_XM
.globl Isr_XF
.extern XM_handler
Isr_XM:
ISR_XF:
 pushq $0
 pushq %rax
 leaq XM_handler(%rip), %rax
 jmp IsrLauncher

.globl Isr_VE
.extern VE_handler
Isr_VE:
 pushq $0
 pushq %rax
 leaq VE_handler(%rip), %rax
 jmp IsrLauncher

.globl Isr_CE
.extern CE_handler
Isr_CE:
 pushq %rax
 leaq VE_handler(%rip), %rax
 jmp IsrLauncher

.globl Isr_HV
.extern HV_handler
Isr_HV:
 pushq $0
 pushq %rax
 leaq HV_handler(%rip), %rax
 jmp IsrLauncher

.globl Isr_VC
.extern VC_handler
Isr_VC:
 pushq %rax
 leaq VC_handler(%rip), %rax
 jmp IsrLauncher

.globl Isr_SX
.extern SX_handler
Isr_SX:
 pushq %rax
 leaq SX_handler(%rip), %rax
 jmp IsrLauncher
