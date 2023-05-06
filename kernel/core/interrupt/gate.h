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
);\	
while(1);													\
return;



extern void DE_handler(ERROR_STACK_PRAM) {
	PRINT_ERR
	PRINT_REG
}
extern void DB_handler(ERROR_STACK_PRAM) {
	PRINT_ERR
	PRINT_REG
}
extern void NMI_handler(ERROR_STACK_PRAM) {
	PRINT_ERR
	PRINT_REG
}
extern void BP_handler(ERROR_STACK_PRAM) {
	PRINT_ERR
	PRINT_REG
}
extern void OF_handler(ERROR_STACK_PRAM) {
	PRINT_ERR
	PRINT_REG
}
extern void BR_handler(ERROR_STACK_PRAM) {
	PRINT_ERR
	PRINT_REG
}
extern void UD_handler(ERROR_STACK_PRAM) {
	PRINT_ERR
	PRINT_REG
}
extern void NM_handler(ERROR_STACK_PRAM) {
	PRINT_ERR
	PRINT_REG
}
extern void DF_handler(ERROR_STACK_PRAM) {
	PRINT_ERR
	PRINT_REG
}
extern void FPU_SEG_ERR_handler(ERROR_STACK_PRAM) {
	PRINT_ERR
	PRINT_REG
}
extern void TS_handler(ERROR_STACK_PRAM) {
	PRINT_ERR
	PRINT_REG
}
extern void NP_handler(ERROR_STACK_PRAM) {
	PRINT_ERR
	PRINT_REG
}
extern void SS_handler(ERROR_STACK_PRAM) {
	PRINT_ERR
	PRINT_REG
}
extern void GP_handler(ERROR_STACK_PRAM) {
	PRINT_ERR
	PRINT_REG
}
extern void PF_handler(ERROR_STACK_PRAM) {
	PRINT_ERR
	PRINT_REG
}
extern void MF_handler(ERROR_STACK_PRAM) {
	PRINT_ERR
	PRINT_REG
}
extern void AC_handler(ERROR_STACK_PRAM) {
	PRINT_ERR
	PRINT_REG
}
extern void MC_handler(ERROR_STACK_PRAM) {
	PRINT_ERR
	PRINT_REG
}
//aka XF
extern void XM_handler(ERROR_STACK_PRAM) {
	PRINT_ERR
	PRINT_REG
}
extern void VE_handler(ERROR_STACK_PRAM) {
	PRINT_ERR
	PRINT_REG
}
extern void CP_handler(ERROR_STACK_PRAM) {
	PRINT_ERR
	PRINT_REG
}
extern void HV_handler(ERROR_STACK_PRAM) {
	PRINT_ERR
	PRINT_REG
}
extern void VC_handler(ERROR_STACK_PRAM) {
	PRINT_ERR
	PRINT_REG
}
extern void SX_handler(ERROR_STACK_PRAM) {
	PRINT_ERR
	PRINT_REG
}

