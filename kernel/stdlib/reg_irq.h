#ifndef __REG_IRQ_H__
#define __REG_IRQ_H__

extern void SetIdtEntry();
extern void SetTssTable();


#define INT_GATE 0xe
#define TRAP_GATE 0xf
#define DPL_SYS 0
#define DPL_USR 3
#define INT_IST 1
#define IST_SSP1 1
#define IST_SSP2 2
#define IST_SSP3 3
#define IST_SSP4 4
#define IST_SSP5 5
#define IST_SSP6 6
#define IST_SSP7 7
#define KERNEL_CODE64_SELECTOR 0x8
#define USER_CODE64_SELECTOR 0x28

#define RegSystemTrap(index, handler_address,ist) \
    SetIdtEntry(index, handler_address, KERNEL_CODE64_SELECTOR, DPL_SYS, TRAP_GATE, ist)

#define RegSyscall(index, handler_address, ist) \
    SetIdtEntry(index, handler_address, USER_CODE64_SELECTOR, DPL_USR, TRAP_GATE, ist)

#define RegUserInt(index, handler_address, ist) \
    SetIdtEntry(index, handler_address, USER_CODE64_SELECTOR, DPL_USR, INT_GATE, ist)


#endif