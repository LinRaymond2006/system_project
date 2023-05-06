#include "printf.h"
#include <stddef.h>
typedef unsigned long size_t;
typedef long ssize_t;
typedef size_t reg_sz64;
#define ERROR_STACK_PRAM reg_sz64 rsp, reg_sz64 errcode
#define PRINT_ERR 												\
	unsigned long *rip = (unsigned long *)(rsp + 0x98);			\
    printf("%s sourced from %p, with error code %p and stack pointer %p\n", __PRETTY_FUNCTION__, *rip, errcode, rsp);                  \
	while(1);													\
	return;

extern void DE_handler(ERROR_STACK_PRAM) {
	PRINT_ERR
}
extern void DB_handler(ERROR_STACK_PRAM) {
	PRINT_ERR
}
extern void NMI_handler(ERROR_STACK_PRAM) {
	PRINT_ERR
}
extern void BP_handler(ERROR_STACK_PRAM) {
	PRINT_ERR
}
extern void OF_handler(ERROR_STACK_PRAM) {
	PRINT_ERR
}
extern void BR_handler(ERROR_STACK_PRAM) {
	PRINT_ERR
}
extern void UD_handler(ERROR_STACK_PRAM) {
	PRINT_ERR
}
extern void NM_handler(ERROR_STACK_PRAM) {
	PRINT_ERR
}
extern void DF_handler(ERROR_STACK_PRAM) {
	PRINT_ERR
}
extern void FPU_SEG_ERR_handler(ERROR_STACK_PRAM) {
	PRINT_ERR
}
extern void TS_handler(ERROR_STACK_PRAM) {
	PRINT_ERR
}
extern void NP_handler(ERROR_STACK_PRAM) {
	PRINT_ERR
}
extern void SS_handler(ERROR_STACK_PRAM) {
	PRINT_ERR
}
extern void GP_handler(ERROR_STACK_PRAM) {
	PRINT_ERR
}
extern void PF_handler(ERROR_STACK_PRAM) {
	PRINT_ERR
}
extern void MF_handler(ERROR_STACK_PRAM) {
	PRINT_ERR
}
extern void AC_handler(ERROR_STACK_PRAM) {
	PRINT_ERR
}
extern void MC_handler(ERROR_STACK_PRAM) {
	PRINT_ERR
}
//aka XF
extern void XM_handler(ERROR_STACK_PRAM) {
	PRINT_ERR
}
extern void VE_handler(ERROR_STACK_PRAM) {
	PRINT_ERR
}
extern void CP_handler(ERROR_STACK_PRAM) {
	PRINT_ERR
}
extern void HV_handler(ERROR_STACK_PRAM) {
	PRINT_ERR
}
extern void VC_handler(ERROR_STACK_PRAM) {
	PRINT_ERR
}
extern void SX_handler(ERROR_STACK_PRAM) {
	PRINT_ERR
}

