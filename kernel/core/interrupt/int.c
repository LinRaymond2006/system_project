#include "gate.h"
#include "fault_irq.h"
#include "fault.h"

typedef struct {
    unsigned long pad[2];
} STRUCT_IDT;
typedef struct {
    unsigned long pad[1];
} STRUCT_GDT;



extern IDT_Table;

#define KERNEL_CODE64 0x8
#define USER_CODE64 0x28

void intit_sysirq() {
    set_ivte(&IDT_Table, EXCEPTION_DE, &handler_DE, KERNEL_CODE64, 1, GATE_TRAP, DPL_SYS);
    set_ivte(&IDT_Table, EXCEPTION_DB, &handler_DB, KERNEL_CODE64, 1, GATE_TRAP, DPL_SYS);
    set_ivte(&IDT_Table, EXCEPTION_NMI, &handler_NMI, KERNEL_CODE64, 1, GATE_INT, DPL_SYS);
    set_ivte(&IDT_Table, EXCEPTION_BP, &handler_BP, KERNEL_CODE64, 1, GATE_TRAP, DPL_USR);
    set_ivte(&IDT_Table, EXCEPTION_OF, &handler_OF, KERNEL_CODE64, 1, GATE_TRAP, DPL_USR);
    set_ivte(&IDT_Table, EXCEPTION_BR, &handler_BR, KERNEL_CODE64, 1, GATE_TRAP, DPL_USR);
    set_ivte(&IDT_Table, EXCEPTION_UD, &handler_UD, KERNEL_CODE64, 1, GATE_TRAP, DPL_SYS);
    set_ivte(&IDT_Table, EXCEPTION_NM, &handler_NM, KERNEL_CODE64, 1, GATE_TRAP, DPL_SYS);
    set_ivte(&IDT_Table, EXCEPTION_DF, &handler_DF, KERNEL_CODE64, 1, GATE_TRAP, DPL_SYS);
    set_ivte(&IDT_Table, EXCEPTION_FPU_SEG_ERR, &handler_FPU_SEG_ERR, KERNEL_CODE64, 1, GATE_TRAP, DPL_SYS);
    set_ivte(&IDT_Table, EXCEPTION_TS, &handler_TS, KERNEL_CODE64, 1, GATE_TRAP, DPL_SYS);
    set_ivte(&IDT_Table, EXCEPTION_NP, &handler_NP, KERNEL_CODE64, 1, GATE_TRAP, DPL_SYS);
    set_ivte(&IDT_Table, EXCEPTION_SS, &handler_SS, KERNEL_CODE64, 1, GATE_TRAP, DPL_SYS);
    set_ivte(&IDT_Table, EXCEPTION_GP, &handler_GP, KERNEL_CODE64, 1, GATE_TRAP, DPL_SYS);
    set_ivte(&IDT_Table, EXCEPTION_PF, &handler_PF, KERNEL_CODE64, 1, GATE_TRAP, DPL_SYS);
    //15: RESERVED
    set_ivte(&IDT_Table, EXCEPTION_MF, &handler_MF, KERNEL_CODE64, 1, GATE_TRAP, DPL_SYS);
    set_ivte(&IDT_Table, EXCEPTION_AC, &handler_AC, KERNEL_CODE64, 1, GATE_TRAP, DPL_SYS);
    set_ivte(&IDT_Table, EXCEPTION_XM, &handler_XM, KERNEL_CODE64, 1, GATE_TRAP, DPL_SYS);
    set_ivte(&IDT_Table, EXCEPTION_VE, &handler_VE, KERNEL_CODE64, 1, GATE_TRAP, DPL_SYS);
    set_ivte(&IDT_Table, EXCEPTION_CP, &handler_CP, KERNEL_CODE64, 1, GATE_TRAP, DPL_SYS);
    //22~17: RESERVED
    set_ivte(&IDT_Table, EXCEPTION_HV, &handler_HV, KERNEL_CODE64, 1, GATE_TRAP, DPL_SYS);
    set_ivte(&IDT_Table, EXCEPTION_VC, &handler_VC, KERNEL_CODE64, 1, GATE_TRAP, DPL_SYS);
    set_ivte(&IDT_Table, EXCEPTION_SX, &handler_SX, KERNEL_CODE64, 1, GATE_TRAP, DPL_SYS);
    //31: RESERVED

}