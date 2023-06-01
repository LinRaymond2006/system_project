#include "printf.h"
#include "reg_irq.h"
#include "fault_irq.h"
typedef unsigned long size_t;
typedef long ssize_t;
typedef size_t reg_sz64;
#define ERROR_STACK_PRAM reg_sz64 rsp, reg_sz64 errcode

#define CLI __asm__ __volatile__ ("cli":::);
#define STI __asm__ __volatile__ ("sti":::);


#define PRINT_ERR 											\
reg_sz64 *rip = (unsigned long *)(rsp + 0x98);				\
reg_sz64 *es = (reg_sz64 *)(rsp + 0x00);				\
reg_sz64 *ds = (reg_sz64 *)(rsp + 0x08);				\
reg_sz64 *r15 = (reg_sz64 *)(rsp + 0x10);				\
reg_sz64 *r14 = (reg_sz64 *)(rsp + 0x18);				\
reg_sz64 *r13 = (reg_sz64 *)(rsp + 0x20);				\
reg_sz64 *r12 = (reg_sz64 *)(rsp + 0x28);				\
reg_sz64 *r11 = (reg_sz64 *)(rsp + 0x30);				\
reg_sz64 *r10 = (reg_sz64 *)(rsp + 0x38);				\
reg_sz64 *r9  = (reg_sz64 *)(rsp + 0x40);				\
reg_sz64 *r8  = (reg_sz64 *)(rsp + 0x48);				\
reg_sz64 *rbp = (reg_sz64 *)(rsp + 0x50);				\
reg_sz64 *rdi = (reg_sz64 *)(rsp + 0x58);				\
reg_sz64 *rsi = (reg_sz64 *)(rsp + 0x60);				\
reg_sz64 *rdx = (reg_sz64 *)(rsp + 0x68);				\
reg_sz64 *rcx = (reg_sz64 *)(rsp + 0x70);				\
reg_sz64 *rbx = (reg_sz64 *)(rsp + 0x78);				\
reg_sz64 *rax = (reg_sz64 *)(rsp + 0x80);				\
printf("%s sourced from %p, with error code %p and stack pointer %p\n", __PRETTY_FUNCTION__, *rip, errcode, rsp);

#define PRINT_REG	\
printf( \
"\
GPREG:\n\
  RAX: %p    RBX: %p    RCX: %p    RDX: %p\n\
  RSI: %p    RDI: %p    RBP: %p    R8:  %p\n\
  R9:  %p    R10: %p    R11: %p    R12: %p\n\
  R13: %p    R14: %p    R15: %p\n\
SREG:\n\
  DS:  %p    ES:  %p\n\
",\
*rax, *rbx, *rcx, *rdx, \
*rsi, *rdi, *rbp, *r8,  \
*r9 , *r10, *r11, *r12, \
*r13, *r14, *r15,      \
*ds, *es              \
);

#define PRINT_SELECTOR_ERR	\
printf("segment selector index: %d, type: ", errcode&0b1111111111000);	\
switch ((errcode>>1)&0b11) {	\
	case 0b00:	\
		printf("GDT (0b00)");	\
		break;	\
	case 0b10:	\
		printf("LDT (0b10)");	\
		break;	\
	default:	\
		printf("IDT (0b01 / 0b11)");	\
}	\
printf(", originated by external event: %s\n", ((errcode&0b1) ? "true" : "false"));



#define HALT_HANDLER __asm__ __volatile__ ("jmp .":::);
//#define HALT_HANDLER while (1) ;


//check out intel 64 and IA-32 Architectures Software Developer’s Manual Vol 3A, chapter 6.14.1 for a refernce



extern void DE_handler(ERROR_STACK_PRAM) {
	PRINT_ERR
	PRINT_REG
	HALT_HANDLER
}
extern void DB_handler(ERROR_STACK_PRAM) {
	PRINT_ERR
	PRINT_REG
	HALT_HANDLER
}
extern void NMI_handler(ERROR_STACK_PRAM) {
	PRINT_ERR
	PRINT_REG
	HALT_HANDLER
}
extern void BP_handler(ERROR_STACK_PRAM) {
	PRINT_ERR
	PRINT_REG
	HALT_HANDLER
}
extern void OF_handler(ERROR_STACK_PRAM) {
	PRINT_ERR
	PRINT_REG
	HALT_HANDLER
}
extern void BR_handler(ERROR_STACK_PRAM) {
	PRINT_ERR
	PRINT_REG
	HALT_HANDLER
}
extern void UD_handler(ERROR_STACK_PRAM) {
	PRINT_ERR
	PRINT_REG
	HALT_HANDLER
}
extern void NM_handler(ERROR_STACK_PRAM) {
	PRINT_ERR
	PRINT_REG
	HALT_HANDLER
}
extern void DF_handler(ERROR_STACK_PRAM) {
	PRINT_ERR
	PRINT_REG
	HALT_HANDLER
}
extern void FPU_SEG_ERR_handler(ERROR_STACK_PRAM) {
	PRINT_ERR
	PRINT_REG
	HALT_HANDLER
}
extern void TS_handler(ERROR_STACK_PRAM) {
	PRINT_ERR
	PRINT_SELECTOR_ERR
	PRINT_REG
	HALT_HANDLER
}
extern void NP_handler(ERROR_STACK_PRAM) {
	PRINT_ERR
	PRINT_SELECTOR_ERR
	PRINT_REG
	HALT_HANDLER
}
extern void SS_handler(ERROR_STACK_PRAM) {
	PRINT_ERR
	PRINT_SELECTOR_ERR
	PRINT_REG
	HALT_HANDLER
}
extern void GP_handler(ERROR_STACK_PRAM) {
	/*
	1.segment error (invalid settings of descriptor fields, refererncing a null descriptor...)
	2.privilleged instruction execution from the user ring
	3.attemping to write value into reserved register
	4.invalid value combinations
	*/
	PRINT_ERR
	PRINT_SELECTOR_ERR
	PRINT_REG
	HALT_HANDLER
}
extern void PF_handler(ERROR_STACK_PRAM) {
	unsigned long reg_cr2;
	//with a different erro code format
	PRINT_ERR

	printf("reason: %s\n", ((errcode&0b1) ? "page protection violation" : "page not presented"));
	printf("  activity:                       %s\n", ((errcode&0b10) ? "write attempted" : "read attemped"));
	printf("  ring privillege:                %s\n", ((errcode&0b100) ? "user (cpl = 3)" : "supervisor (cpl = 0)"));
	asm volatile("mov %%cr2, %0" :"=r" (reg_cr2)::);
	printf("  faulty address accessed (CR2):  %p\n", reg_cr2);
	if (errcode&0b1000) {
		//if the reserved write: cr4.pse or cr4.pae and writing reserved field
		printf("  INFORMATION: writing to reserved field attempted\n");
	}
	if (errcode&0b10000) {
		printf("  INFORMATION: instruction fetch error, execution on NX pages\n");
	}
	if (errcode&0b100000) {
		//check PKRU MSR (user mode) or PKRS MSR (system mode)
		printf("  INFORMATION: protection key violation\n");
	}
	if (errcode&0b1000000) {
		printf("  INFORMATION: error accessing shadow stack\n");
	}
	if (errcode&0b1000000000000000) {
		//SGX violation has nothing to do with the oridinary page fault
		printf("  INFORMATION: SGX (Software Guard eXtension) voilation\n");
	}

	PRINT_REG
	HALT_HANDLER
}
extern void MF_handler(ERROR_STACK_PRAM) {
	PRINT_ERR
	PRINT_REG
	HALT_HANDLER
}
extern void AC_handler(ERROR_STACK_PRAM) {
	PRINT_ERR
	PRINT_REG
	HALT_HANDLER
}
extern void MC_handler(ERROR_STACK_PRAM) {
	PRINT_ERR
	PRINT_REG
	HALT_HANDLER
}
//aka XF
extern void XM_handler(ERROR_STACK_PRAM) {
	PRINT_ERR
	PRINT_REG
	HALT_HANDLER
}
extern void VE_handler(ERROR_STACK_PRAM) {
	PRINT_ERR
	PRINT_REG
	HALT_HANDLER
}
extern void CP_handler(ERROR_STACK_PRAM) {
	PRINT_ERR
	PRINT_REG
	HALT_HANDLER
}
extern void HV_handler(ERROR_STACK_PRAM) {
	PRINT_ERR
	PRINT_REG
	HALT_HANDLER
}
extern void VC_handler(ERROR_STACK_PRAM) {
	PRINT_ERR
	PRINT_REG
	HALT_HANDLER
}
extern void SX_handler(ERROR_STACK_PRAM) {
	PRINT_ERR
	PRINT_REG
	HALT_HANDLER
}



//some extern stuff here

#define INT_GATE 0xe
#define TRAP_GATE 0xf
#define DPL_SYS 0
#define DPL_USR 3
#define INT_IST 1
#define IST_SSP1 1
#define IST_SSP2 2
#define IST_SSP3 3
#define IST_SSP4 4
#define IST_SSP5 5
#define IST_SSP6 6
#define IST_SSP7 7
#define KERNEL_CODE_SELECTOR 0x8



//set_intr_gate: DPL0, TYPE: INT
//set_trap_gate: DPL0, TYPE: TRAP
//set_system_gate: DPL3, TYPE: TRAP (USER MODE)
extern void Isr_DE();
extern void Isr_DB();
extern void Isr_NMI();
extern void Isr_BP();
extern void Isr_OF();
extern void Isr_BR();
extern void Isr_UD();
extern void Isr_NM();
extern void Isr_DF();
extern void Isr_FPU_SEG_ERR();
extern void Isr_TS();
extern void Isr_NP();
extern void Isr_SS();
extern void Isr_GP();
extern void Isr_PF();
extern void Isr_MF();
extern void Isr_AC();
extern void Isr_MC();
extern void Isr_XM();
extern void Isr_VE();
extern void Isr_CP();
extern void Isr_HV();
extern void Isr_VC();
extern void Isr_SX();

#define IST1SPACE_SZ (8*1024)

extern char Ist1Space[IST1SPACE_SZ] = {0};

#define IST1END (Ist1Space+IST1SPACE_SZ)

extern void RegisterHandlerIrq() {
	//the B (busy) bit will be set after a TSS is loaded into TSR, and loading it again will lead to a #TS exception
	printf("initializing TSS table\n");
    SetTssTable(0xffff800000007c00, IST1END, 0xffff800000007c00, 0xffff800000007c00, 0xffff800000007c00, 0xffff800000007c00, 0xffff800000007c00, 0xffff800000007c00, 0xffff800000007c00, 0xffff800000007c00);
	printf("done\n");
	printf("initializing exception handler\n");
    SetIdtEntry(EXCEPTION_DE, (unsigned long)&Isr_DE, KERNEL_CODE_SELECTOR, DPL_SYS, TRAP_GATE, INT_IST);
    SetIdtEntry(EXCEPTION_DB, (unsigned long)&Isr_DB, KERNEL_CODE_SELECTOR, DPL_SYS, TRAP_GATE, INT_IST);
    SetIdtEntry(EXCEPTION_NMI, (unsigned long)&Isr_NMI, KERNEL_CODE_SELECTOR, DPL_SYS, INT_GATE, INT_IST);
    SetIdtEntry(EXCEPTION_BP, (unsigned long)&Isr_BP, KERNEL_CODE_SELECTOR, DPL_USR, TRAP_GATE, INT_IST);
    SetIdtEntry(EXCEPTION_OF, (unsigned long)&Isr_OF, KERNEL_CODE_SELECTOR, DPL_USR, TRAP_GATE, INT_IST);
    SetIdtEntry(EXCEPTION_BR, (unsigned long)&Isr_BR, KERNEL_CODE_SELECTOR, DPL_USR, TRAP_GATE, INT_IST);
    SetIdtEntry(EXCEPTION_UD, (unsigned long)&Isr_UD, KERNEL_CODE_SELECTOR, DPL_SYS, TRAP_GATE, INT_IST);
    SetIdtEntry(EXCEPTION_NM, (unsigned long)&Isr_NM, KERNEL_CODE_SELECTOR, DPL_SYS, TRAP_GATE, INT_IST);
    SetIdtEntry(EXCEPTION_DF, (unsigned long)&Isr_DF, KERNEL_CODE_SELECTOR, DPL_SYS, TRAP_GATE, INT_IST);
    SetIdtEntry(EXCEPTION_FPU_SEG_ERR, (unsigned long)&Isr_FPU_SEG_ERR, KERNEL_CODE_SELECTOR, DPL_SYS, TRAP_GATE, INT_IST);
    SetIdtEntry(EXCEPTION_TS, (unsigned long)&Isr_TS, KERNEL_CODE_SELECTOR, DPL_SYS, TRAP_GATE, INT_IST);
    SetIdtEntry(EXCEPTION_NP, (unsigned long)&Isr_NP, KERNEL_CODE_SELECTOR, DPL_SYS, TRAP_GATE, INT_IST);
    SetIdtEntry(EXCEPTION_SS, (unsigned long)&Isr_SS, KERNEL_CODE_SELECTOR, DPL_SYS, TRAP_GATE, INT_IST);
    SetIdtEntry(EXCEPTION_GP, (unsigned long)&Isr_GP, KERNEL_CODE_SELECTOR, DPL_SYS, TRAP_GATE, INT_IST);
    SetIdtEntry(EXCEPTION_PF, (unsigned long)&Isr_PF, KERNEL_CODE_SELECTOR, DPL_SYS, TRAP_GATE, INT_IST);
    SetIdtEntry(EXCEPTION_MF, (unsigned long)&Isr_MF, KERNEL_CODE_SELECTOR, DPL_SYS, TRAP_GATE, INT_IST);
    SetIdtEntry(EXCEPTION_AC, (unsigned long)&Isr_AC, KERNEL_CODE_SELECTOR, DPL_SYS, TRAP_GATE, INT_IST);
    SetIdtEntry(EXCEPTION_MC, (unsigned long)&Isr_MC, KERNEL_CODE_SELECTOR, DPL_SYS, TRAP_GATE, INT_IST);
    SetIdtEntry(EXCEPTION_XM, (unsigned long)&Isr_XM, KERNEL_CODE_SELECTOR, DPL_SYS, TRAP_GATE, INT_IST);
    SetIdtEntry(EXCEPTION_VE, (unsigned long)&Isr_VE, KERNEL_CODE_SELECTOR, DPL_SYS, TRAP_GATE, INT_IST);
    SetIdtEntry(EXCEPTION_CP, (unsigned long)&Isr_CP, KERNEL_CODE_SELECTOR, DPL_SYS, TRAP_GATE, INT_IST);
    SetIdtEntry(EXCEPTION_HV, (unsigned long)&Isr_HV, KERNEL_CODE_SELECTOR, DPL_SYS, TRAP_GATE, INT_IST);
    SetIdtEntry(EXCEPTION_VC, (unsigned long)&Isr_VC, KERNEL_CODE_SELECTOR, DPL_SYS, TRAP_GATE, INT_IST);
    SetIdtEntry(EXCEPTION_SX, (unsigned long)&Isr_SX, KERNEL_CODE_SELECTOR, DPL_SYS, TRAP_GATE, INT_IST);
	printf("done\n");
	return;
}