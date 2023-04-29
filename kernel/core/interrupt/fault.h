#ifndef __FAULT_H__
#define __FAULT_H__

#include "printf.h"

typedef unsigned long size_t;
typedef long ssize_t;
typedef size_t reg_sz64;

#define ERROR_STACK_IST1 reg_sz64 tss_rsp, reg_sz64 errcode
#define RIP_IST1         (reg_sz64 *)(tss_rsp+0x98)
#define HALT_FOREVER()        while (1) {;}
#define GERNIC_FUNCTION_HANDLER()            \
    reg_sz64 rip=*RIP_IST1;                                                                                                             \
    printf("%s sourced from %p, with error code %p and stack pointer %p", __PRETTY_FUNCTION__, rip, errcode, tss_rsp);                  \
    HALT_FOREVER();                                                                                                                     \
    return;

//
void handler_DE(ERROR_STACK_IST1) {
    GERNIC_FUNCTION_HANDLER()
}
void handler_DB(ERROR_STACK_IST1) {
    GERNIC_FUNCTION_HANDLER()
}
void handler_NMI(ERROR_STACK_IST1) {
    GERNIC_FUNCTION_HANDLER()
}
void handler_BP(ERROR_STACK_IST1) {
    GERNIC_FUNCTION_HANDLER()
}
void handler_OF(ERROR_STACK_IST1) {
    GERNIC_FUNCTION_HANDLER()
}
void handler_BR(ERROR_STACK_IST1) {
    GERNIC_FUNCTION_HANDLER()
}
void handler_UD(ERROR_STACK_IST1) {
    GERNIC_FUNCTION_HANDLER()
}
void handler_NM(ERROR_STACK_IST1) {
    GERNIC_FUNCTION_HANDLER()
}
void handler_DF(ERROR_STACK_IST1) {
    GERNIC_FUNCTION_HANDLER()
}
void handler_FPU_SEG_ERR(ERROR_STACK_IST1) {
    GERNIC_FUNCTION_HANDLER()
}
void handler_TS(ERROR_STACK_IST1) {
    //need special handler
}
void handler_NP(ERROR_STACK_IST1) {
    //need special handler
}
void handler_SS(ERROR_STACK_IST1) {
    //need special handler
}
void handler_GP(ERROR_STACK_IST1) {}
void handler_PF(ERROR_STACK_IST1) {}
//15 is reserved
void handler_MF(ERROR_STACK_IST1) {
    GERNIC_FUNCTION_HANDLER()
}
void handler_AC(ERROR_STACK_IST1) {
    GERNIC_FUNCTION_HANDLER()
}
void handler_MC(ERROR_STACK_IST1) {
    GERNIC_FUNCTION_HANDLER()
}
void handler_XM(ERROR_STACK_IST1) {
    GERNIC_FUNCTION_HANDLER()
}
void handler_VE(ERROR_STACK_IST1) {
    GERNIC_FUNCTION_HANDLER()
}
void handler_CP(ERROR_STACK_IST1) {
    GERNIC_FUNCTION_HANDLER()
}
//22~27 are reserved
void handler_HV(ERROR_STACK_IST1) {
    GERNIC_FUNCTION_HANDLER()
}
void handler_VC(ERROR_STACK_IST1) {
    GERNIC_FUNCTION_HANDLER()
}
void handler_SX(ERROR_STACK_IST1) {
    GERNIC_FUNCTION_HANDLER()
}


//31 is reserved


#endif