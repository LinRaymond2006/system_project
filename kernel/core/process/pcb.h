#ifndef __PCB_H__
#define __PCB_H__

#include "stdlist.h"
#include "linkage.h"

typedef struct {
    //DL_Node *prevRegion;
    //DL_Node *nextRegion;
    SL_Node *nextRegion;
    void *Base;
    void *Limit;
    long Attr;

} MemoryRegion;

#define DATA_SECTION_INDEX   0
#define TEXT_SECTION_INDEX   1
#define RODATA_SECTION_INDEX 2
#define HEAP_SECTION_INDEX   3
#define STACK_SECTION_INDEX  4
#define BSS_SECTION_INDEX    5

typedef struct {
    MemoryRegion Section[16];
} MemoryMap ATTR_PACKED;

typedef struct {
    unsigned long Pid;
    MemoryMap VmMap;
    unsigned long ProcStatus;
    unsigned long ProcFlags;
} ProcInfo ATTR_PACKED;

typedef struct {
    unsigned long Kernel_Rsp;
    unsigned long Reg_Rip;
    unsigned long Reg_Rsp;
    unsigned long Sreg_Fs;
    unsigned long Sreg_Gs;
    unsigned long SegFaultAddr;
    //the irq which trap the process
    unsigned long IrqTrap;
    unsigned long ErrCode;
} TaskStateRegs ATTR_PACKED;

struct Pcb_Struct {
    DL_Node ContextNode;
    TaskStateRegs *TSS_Reg;
    
};


#endif