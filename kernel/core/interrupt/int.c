#include "fault_irq.h"
#include "fault.h"
#include "gate.h"

typedef struct {
    unsigned long pad[2];
} STRUCT_IDT;
typedef struct {
    unsigned long pad[1];
} STRUCT_GDT;



extern STRUCT_IDT IDT_TABLE;

#define KERNEL_CODE64 0x8
#define USER_CODE64 0X28

//not finished yet!
void intit_sysirq() {
    set_ivte(&IDT_TABLE, EXCEPTION_DE, &handler_DE, KERNEL_CODE64, 1, GATE_TRAP, DPL_SYS);
    set_ivte(&IDT_TABLE, EXCEPTION_DB, &handler_DB, KERNEL_CODE64, 1, GATE_TRAP, DPL_SYS);
    set_ivte(&IDT_TABLE, EXCEPTION_NMI, &handler_NMI, KERNEL_CODE64, 1, GATE_INT, DPL_SYS);
    set_ivte(&IDT_TABLE, EXCEPTION_BP, &handler_BP, KERNEL_CODE64, 1, GATE_TRAP, DPL_SYS);
    set_ivte(&IDT_TABLE, EXCEPTION_OF, &handler_OF, KERNEL_CODE64, 1, GATE_TRAP, DPL_SYS);
    set_ivte(&IDT_TABLE, EXCEPTION_BR, &handler_BR, KERNEL_CODE64, 1, GATE_TRAP, DPL_SYS);
    set_ivte(&IDT_TABLE, EXCEPTION_UD, &handler_UD, KERNEL_CODE64, 1, GATE_TRAP, DPL_SYS);
    set_ivte(&IDT_TABLE, EXCEPTION_NM, &handler_NM, KERNEL_CODE64, 1, GATE_TRAP, DPL_SYS);
    set_ivte(&IDT_TABLE, EXCEPTION_DF, &handler_DF, KERNEL_CODE64, 1, GATE_TRAP, DPL_SYS);
    set_ivte(&IDT_TABLE, EXCEPTION_FPU_SEG_ERR, &handler_FPU_SEG_ERR, KERNEL_CODE64, 1, GATE_TRAP, DPL_SYS);
    set_ivte(&IDT_TABLE, EXCEPTION_TS, &handler_TS, KERNEL_CODE64, 1, GATE_TRAP, DPL_SYS);
    set_ivte(&IDT_TABLE, EXCEPTION_NP, &handler_NP, KERNEL_CODE64, 1, GATE_TRAP, DPL_SYS);
    set_ivte(&IDT_TABLE, EXCEPTION_SS, &handler_SS, KERNEL_CODE64, 1, GATE_TRAP, DPL_SYS);
    set_ivte(&IDT_TABLE, EXCEPTION_GP, &handler_GP, KERNEL_CODE64, 1, GATE_TRAP, DPL_SYS);
    set_ivte(&IDT_TABLE, EXCEPTION_PF, &handler_PF, KERNEL_CODE64, 1, GATE_TRAP, DPL_SYS);
    //15: RESERVED
    set_ivte(&IDT_TABLE, EXCEPTION_MF, &handler_MF, KERNEL_CODE64, 1, GATE_TRAP, DPL_SYS);
    set_ivte(&IDT_TABLE, EXCEPTION_AC, &handler_AC, KERNEL_CODE64, 1, GATE_TRAP, DPL_SYS);
    set_ivte(&IDT_TABLE, EXCEPTION_XM, &handler_XM, KERNEL_CODE64, 1, GATE_TRAP, DPL_SYS);
    set_ivte(&IDT_TABLE, EXCEPTION_VE, &handler_VE, KERNEL_CODE64, 1, GATE_TRAP, DPL_SYS);
    set_ivte(&IDT_TABLE, EXCEPTION_CP, &handler_CP, KERNEL_CODE64, 1, GATE_TRAP, DPL_SYS);
    //22~17: RESERVED
    set_ivte(&IDT_TABLE, EXCEPTION_HV, &handler_HV, KERNEL_CODE64, 1, GATE_TRAP, DPL_SYS);
    set_ivte(&IDT_TABLE, EXCEPTION_VC, &handler_VC, KERNEL_CODE64, 1, GATE_TRAP, DPL_SYS);
    set_ivte(&IDT_TABLE, EXCEPTION_SX, &handler_SX, KERNEL_CODE64, 1, GATE_TRAP, DPL_SYS);
    //31: RESERVED

    refresh_idtr();
}