#ifndef __PCB_H__
#define __PCB_H__

#include "regstruct.h"

//maybe use interrrupt to switch context
//interrupt will be fired by programmable timer

//TCB (thread control block) is different from PCB (process control block), and the TCB may be implemented in userland

#define MAX_SEGMENT_NR 32

//"volatile" keyword: the value may be change by unexpected source

//the flags are for reference only, they have no effect on the memory management
#define VALID_SEGMENT (1<<0)
#define SEGMENT_R     (1<<1)
#define SEGMENT_W     (1<<2)
#define SEGMENT_X     (1<<3)
#define SEGMENT_USR   (1<<4)
#define SEGMENT_SYS   (0<<4)



//the RWX is still implemented by the paging machanism
struct PcbSegDesc {
    void *start;
    void *end;
    char attr;  //is_valid, dpl(USER/SYSTEM), RWX, ... (the exact format isn't determined yet)
};

//unsure now
enum ProcStatus {
    PROC_STATUS_CREATED,
    PROC_STATUS_READY,
    PROC_STATUS_RUNNING,
    PROC_STATUS_BLOCKED,
    PROC_STATUS_TERMINATED,
    PROC_STATUS_ZOMBIE,
    PROC_STATUS_SCHEDULED,
    PROC_STATUS_SUSPENDED,
    PROC_STATUS_HIBERNATED
};




typedef struct PcbStruct {
    struct PcbStruct *prev_context;
    struct PcbStruct *next_context;
    unsigned long pid;
    enum ProcStatus status;
    volatile unsigned long flags;
    //scheduling priority
    volatile unsigned long priority;
    unsigned long cr3;  //page table, the kernel space is shared, or the process is unable to jump back to the kernel
    volatile struct RegisterSet regs;
    volatile struct PcbSegDesc procmap[MAX_SEGMENT_NR];
    volatile unsigned long rip;
    volatile unsigned long rsp;
    volatile unsigned long rsp_kernel;
    //do some access control stuff here once the other part of the system is done
} pcb;



#endif