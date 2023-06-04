#include "reg_irq.h"
#include "printf.h"
#include "asm.h"
#include "regstruct.h"
#include "stddef.h"
#include "pcb.h"
#include <stddef.h>
/*

idea about scheduling:

the handler will use a global variable, which will hold how many time slice left for the current process
once the counter reaches zero, scheduler loads the time slices number of next process to be scheduled


*/


//#define SCHEDULER_IRQ 

/*

should context structure uses rounded doubly link list? or a rounded singly link list?
    - a round doubly link list?

0 <-> 1 <-> 2
^           ^
|           |
\----------/


context operations:
    insert
    delete
    a pointer to the last context is needed (FILO structure)
        push
        pop
    grouping the context (priority)
*/

//context (pcb) definition should be in "pcb.h" under the same directory


/*
source code that may be useful in linux 2.6:
    - arch/x86_64/kernel/suspend.c
    - arch/x86_64/kernel/suspend_asm.S
    - include/asm-x86_64/suspend.h


*/


//unsure now
#define PROC_FLAG_KPROC (0<<0)
#define PROC_FLAG_UPROC (1<<0)
#define PROC_FLAG_MAIN_THREAD (0<<1)
#define PROC_FLAG_SUB_THREAD (1<<1)

#define PRIORITY_NOT_IMPELEMENTED_YET 0

inline unsigned long get_cr3() {
    register unsigned long reg_cr3;
    __asm__ __volatile__ ("movq %%cr3, %0" : "=r" (reg_cr3));
    return reg_cr3;
}

#define INIT_PROC_PID 1

#define INIT_PROC_USER_SEG_NR 0
#define INIT_PROC_KERNEL_SEG_NR 1

//just for a test
#define TEST_SCHEDULER_IRQ 0x31

pcb initproc_pcb;
pcb *current_context=&initproc_pcb;

void InitScheduler() {
    //create and set "init" process (pid 0)
    current_context=&initproc_pcb;
    current_context->pid=INIT_PROC_PID;
    current_context->prev_context=NULL;
    current_context->next_context=NULL;
    current_context->status=PROC_STATUS_READY;
    current_context->flags=PROC_FLAG_KPROC | PROC_FLAG_MAIN_THREAD;
    current_context->priority=PRIORITY_NOT_IMPELEMENTED_YET;
    current_context->cr3=get_cr3();
    current_context->procmap[INIT_PROC_USER_SEG_NR].start=0;
    current_context->procmap[INIT_PROC_USER_SEG_NR].end=0x00007fffffffffff;
    current_context->procmap[INIT_PROC_USER_SEG_NR].attr=VALID_SEGMENT | SEGMENT_R | SEGMENT_W | SEGMENT_X | SEGMENT_USR;
    current_context->procmap[INIT_PROC_KERNEL_SEG_NR].start=0xffff800000000000;
    current_context->procmap[INIT_PROC_KERNEL_SEG_NR].end=0xffffffffffffffff;
    current_context->procmap[INIT_PROC_KERNEL_SEG_NR].attr=VALID_SEGMENT | SEGMENT_R | SEGMENT_W | SEGMENT_X | SEGMENT_SYS;
    current_context->rip=NULL;
    current_context->rsp=NULL;
    current_context->rsp_kernel=NULL;

    extern void *switch_context;
    //register the experimental context-switching handler
    RegSystemTrap(TEST_SCHEDULER_IRQ, &switch_context, 1);


    return;
}

void Schedule(pcb *current_context, pcb *target_context) {
    //the schedling function is not implemented yet, scheduler is called throught a trap gate with "int" instruction (for now)

    return;
}
