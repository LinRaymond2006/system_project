#ifndef __FAULT_H__
#define __FAULT_H__

void handler_DE() {}
void handler_DB() {}
void handler_NMI() {}
void handler_BP() {}
void handler_OF() {}
void handler_BR() {}
void handler_UD() {}
void handler_NM() {}
void handler_DF() {}
void handler_FPU_SEG_ERR() {}
void handler_TS() {}
void handler_NP() {}
void handler_SS() {}
void handler_GP() {}
void handler_PF() {}
//15 is reserved
void handler_MF() {}
void handler_AC() {}
void handler_MC() {}
void handler_XM() {}
void handler_VE() {}
void handler_CP() {}
//22~27 are reserved
void handler_HV() {}
void handler_VC() {}
void handler_SX() {}
//31 is reserved


#endif