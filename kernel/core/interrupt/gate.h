#include "printf.h"
#include <stddef.h>
typedef unsigned long size_t;
typedef long ssize_t;
typedef size_t reg_sz64;
#define ERROR_STACK_PRAM reg_sz64 rsp, reg_sz64 errcode

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
  DS:  %p    ES: %p\n\
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



#define HALT_HANDLER while(1);

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
	//with a different erro code format
	PRINT_ERR

	printf("reason:          %s", ((errcode&0b1) ? "page protection violation" : "page not presented"));
	printf("  activity:        %s", ((errcode&0b10) ? "write attempted" : "read attemped"));
	printf("  ring privillege: %s", ((errcode&0b100) ? "user (cpl = 3)" : "non-user (cpl != 3)"));
	//if the reserved write is set: cr4.pse || cr4.pae is set 
	printf("  attempt")

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

