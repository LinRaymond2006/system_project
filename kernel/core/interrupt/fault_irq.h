#ifndef __FAULT_IRQ_H__
#define __FAULT_IRQ_H__

/*      IRQ NUMBER                         |MEMONIC|NAME|TYPE|ERROR CODE EXISTENCE */
#define EXCEPTION_DE                0   // #DE: Division Error, Fault, No error code
#define EXCEPTION_DB                1   // #DB: Debug, Fault/Trap, No error code
#define EXCEPTION_NMI               2   // NMI: Non-maskable Interrupt, Interrupt, N/A
#define EXCEPTION_BP                3   // #BP: Breakpoint, Trap, No error code
#define EXCEPTION_OF                4   // #OF: Overflow, Trap, No error code
#define EXCEPTION_BR                5   // #BR: Bound Range Exceeded, Fault, No error code
#define EXCEPTION_UD                6   // #UD: Invalid Opcode, Fault, No error code
#define EXCEPTION_NM                7   // #NM: Device Not Available, Fault, No error code
#define EXCEPTION_DF                8   // #DF: Double Fault, Abort, Error code present (Zero)
#define EXCEPTION_FPU_SEG_ERR       9   // Coprocessor Segment Overrun, Fault, N/A
#define EXCEPTION_TS                10  // #TS: Invalid TSS, Fault, Error code present
#define EXCEPTION_NP                11  // #NP: Segment Not Present, Fault, Error code present
#define EXCEPTION_SS                12  // #SS: Stack-Segment Fault, Fault, Error code present
#define EXCEPTION_GP                13  // #GP: General Protection Fault, Fault, Error code present
#define EXCEPTION_PF                14  // #PF: Page Fault, Fault, Error code present
#define EXCEPTION_RESERVED15        15  // Reserved, N/A, N/A
#define EXCEPTION_MF                16  // #MF: x87 Floating-Point Exception, Fault, No error code
#define EXCEPTION_AC                17  // #AC: Alignment Check, Fault, Error code present
#define EXCEPTION_MC                18  // #MC: Machine Check, Abort, No error code
#define EXCEPTION_XM                19  // #XM/#XF: SIMD Floating-Point Exception, Fault, No error code
#define EXCEPTION_XF                EXCEPTION_XM  //alias
#define EXCEPTION_VE                20  // #VE: Virtualization Exception, Fault, No error code
#define EXCEPTION_CP                21  // #CP: Control Protection Exception, Fault, Error code present
#define EXCEPTION_RESERVED22        22  // Reserved, N/A, N/A
#define EXCEPTION_RESERVED23        23  // Reserved, N/A, N/A
#define EXCEPTION_RESERVED24        24  // Reserved, N/A, N/A
#define EXCEPTION_RESERVED25        25  // Reserved, N/A, N/A
#define EXCEPTION_RESERVED26        26  // Reserved, N/A, N/A
#define EXCEPTION_RESERVED27        27  // Reserved, N/A, N/A
#define EXCEPTION_HV                28  // #HV: Hypervisor Injection Exception, Fault, No error code
#define EXCEPTION_VC                29  // #VC: VMM Communication Exception, Fault, Error code present
#define EXCEPTION_SX                30  // #SX: Security Exception, Fault, Error code present
#define EXCEPTION_RESERVED31        31  // Reserved, N/A, N/A


//abandoned, only works on legacy 486 processor
//#define EXCEPTION_FERR  13

#endif
