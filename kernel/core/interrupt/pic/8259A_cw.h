#ifndef __8259A_CW_H__
#define __8259A_CW_H__
/*
8259A chipset
    8259A master
        PIN IR0: timer clock
        PIN IR1: keyboard
        PIN IR2: cascaded 8259A
        PIN IR3: serial port 2
        PIN IR4: serial port 1
        PIN IR5: parellel port 2
        PIN IR6: floppy dirver
        PIN IR7: parellel port 1
    8259A slave
        PIN IR0: CMOS RTC real-time clock
        PIN IR1: (connected to master 8259A)
        PIN IR2: RESERVED
        PIN IR3: RESERVED
        PIN IR4: PS/2 mouse
        PIN IR5: coprocessor
        PIN IR6: SATA master
        PIN IR7: SATA slave

*/
#include "asm.h"

#define PORT0_8259A_MASTER 0x20
#define PORT1_8259A_MASTER 0x21

#define MASTER_ICW1_PORT 0x20
#define MASTER_ICW2_PORT 0x21
#define MASTER_ICW3_PORT 0x21
#define MASTER_ICW4_PORT 0x21

#define PORT0_8259A_SLAVE 0xa0
#define PORT1_8259A_SLAVE 0xa1

#define SLAVE_ICW1_PORT 0xa0
#define SLAVE_ICW2_PORT 0xa1
#define SLAVE_ICW3_PORT 0xa1
#define SLAVE_ICW4_PORT 0xa1

//ICW IS CONFIGURED BEFORE A HANDLER IS INVOKED
//REGISTERS: ICW1 ICW2 ICW3 (ICW4 IS OPTIONAL)
//ICW1: 8259A CHIPSET STRUCTURE DEFNIITION
//ICW2: IRQ NUMBER
//ICW3: CASCADE PIN (SLAVE) / PIN MAP (MASTER)
//ICW4: 8259A COFIGURATION

//USING A SINGLE 8259A WITHOUT CONNECTING A EXTRA SLAVE CHIP
#define ICW1_SINGLE_8259A (1<<1)
//CASCADED STRUCTURE
#define ICW1_CASCADED_8259A (0<<1)
//SET IF ICW4 IS PRESENTED
#define ICW1_ICW4_PRESENTED (1<<0)
#define ICW1_NO_ICW4 (0<<0)

//DEFAULT VALUE FOR ICW1
#define ICW1_TYPICAL (ICW1_CASCADED_8259A | ICW1_ICW4_PRESENTED)


//SPECIFIC FULL NESTED MODE, THE MASTER 8259A WILL NOT MASK INTERRUPTS FROM SLAVE 8259A, HANDLER HAVE DETERMINE WETHER TO SEND A EOI (END OF INTERRUPT) COMMAND TO SLAVE 8259A EFORE RETURNING
//USING SFNM, IF ISR REGISTER IS STILL HOLDING OTHER INTERRUPT REQUEST, THERE'S NO NEED TO SEND A EOI COMMAND.OTHERWISE, SEND A EOI
#define ICW4_SFNM (1<<4)
//FULL NESTED MODE, ALL INTERRUPTS FROM SLAVE 8259A WILL BE MASKED BEFORE A INTURRUPT IS FINISH REGARDLESS OF THE PRIORITY
#define ICW4_FNM (0<<4)
//BY USING THIS MODE, ONLY ONE INTERRUPT IS ACCEPT, OTHER INTERRUPTS WILL BE IGNORED WHILE ONE INTERRUPT HANDLER IS RUNNING 
#define ICW4_NO_BUFFERING (0b00<<2)
//NOT SURE
#define ICW4_MASTER_BUFFERING (0b11<<2)
//NOT SURE
#define ICW4_SLAVE_BUFFERING (0b10<<2)
//AUTO ENABLE TO CORRESPOND BIT IN ISR REGISTER AFTER RECIEVING SECOND "INTA" PULSE
#define ICW4_AEOI (1<<1)
//HOST HAVE TO ENABLE CORRESPOND BIT IN ISR REGISTER MANUALLY
#define ICW4_EOI (0<<1)
//FOR 8086/8088
#define ICW4_8086_M (1<<0)
//FOR MCS80/MCS85
#define ICW4_MCS80_M (0<<0)

//DEFAULT VALUE FOR ICW4
#define ICW4_TYPICAL (ICW4_FNM | ICW4_NO_BUFFERING | ICW4_EOI | ICW4_8086_M)


//OCW REGISTERS PROVIDE CONTROL OF INTERRUPT BEHAVIORS DURING RUNTIME (WHILE A HANDLER IS EXECUTING)
//REGISTERS: OCW1 OCW2 OCW3
//OCW1: IRQ MASK (MASK UNUSED IRQ TO AVOID HEAVY TRAFFIC. 0: AVAILABLE, 1:MASKED)
//OCW2: EOI COMMAND REGISTER
//OCW3: 8259A CONFIGURATION
#define PORT0_8259A_MASTER 0x20
#define PORT1_8259A_MASTER 0x21
#define PORT0_8259A_SLAVE 0xa0
#define PORT1_8259A_SLAVE 0xa1

#define MASTER_OCW1_PORT 0x21
#define MASTER_OCW2_PORT 0x20
#define MASTER_OCW3_PORT 0x20
#define SLAVE_OCW1_PORT 0xa1
#define SLAVE_OCW2_PORT 0xa0
#define SLAVE_OCW3_PORT 0xa0

#define _OCW2_COMMAND_R 0b100
#define _OCW2_COMMAND_SL  0b010
#define _OCW2_COMMAND_EOI 0b001

//IF SL BIT IS SET, L1 AND L2 (BIT 2 AND BIT 1) ARE VALID.OTHERWISE, THEY ARE INVALID
//EOI WILL CLEAR CORRESPOND BIT IN ISR

//when OCW2 issues a SPECIFIC EOI command, L2~L0 indicates the priority of the interrupt to be cleared
//when OCW2 issues SPECIFIC priority rotation command, L2~L0 indicate the lowest priority

//Rotation bit: cooperate with SL to decide whether to use automatic rotation or SPECIFIC rotation for interrupt PRIORITY
//EOI: cooperate with SL to decide whether it's a SPECIFIC EOI or a oridinary EOI

/*


+---------------------+--------+-----------------------------------------------------------------------------+
| Command             | Value  | Description                                                                 |
+---------------------+--------+-----------------------------------------------------------------------------+
| Non-specific EOI    | 001    | Clears the highest-priority interrupt request that is currently active on   |
|                     |        | any interrupt line, regardless of its level.                                |
+---------------------+--------+-----------------------------------------------------------------------------+
| Specific EOI        | 011    | Clears the highest-priority interrupt request on the interrupt line         |
|                     |        | specified in bits 2-0 (the IR level).                                       |
+---------------------+--------+-----------------------------------------------------------------------------+
| Non-specific EOI and| 101    | Clears the highest-priority interrupt request that is currently active on   |
| Rotate              |        | any interrupt line, and then rotates the interrupt request levels. Used in  |
|                     |        | Priority Auto-Rotation mode.                                                |
+---------------------+--------+-----------------------------------------------------------------------------+
| Start of Auto-EOI   | 100    | Starts the automatic EOI mode and then rotates the interrupt request levels.|
| and Rotate          |        | Used in Priority Auto-Rotation mode.                                        |
+---------------------+--------+-----------------------------------------------------------------------------+
| Clear Auto-EOI and  | 000    | Clears the automatic EOI mode and then rotates the interrupt request levels.|
| Rotate              |        | Used in Priority Auto-Rotation mode.                                        |
+---------------------+--------+-----------------------------------------------------------------------------+
| Specific EOI and    | 111    | Clears the highest-priority interrupt request on the interrupt line         |
| Rotate              |        | specified in bits 2-0 (the IR level), and then rotates the interrupt request|
|                     |        | levels. Used in Specific Rotation mode.                                     |
+---------------------+--------+-----------------------------------------------------------------------------+
| Set Priority        | 110    | Sets the priority of the interrupt line specified in bits 2-0 (the IR       |
|                     |        | level) to the highest level, and then rotates the interrupt request levels. |
|                     |        | Used in Specific Rotation mode.                                             |
+---------------------+--------+-----------------------------------------------------------------------------+
| Not used            | 010    | This EOI type is not used and must be set to 0.                             |
+---------------------+--------+-----------------------------------------------------------------------------+



*/

/*

#define _OCW2_COMMAND_R 0b100
#define _OCW2_COMMAND_SL  0b010
#define _OCW2_COMMAND_EOI 0b001
*/


//#define OCW2_COMMAND(X) (X<<5)
#define OCW2_PRIORITY(X) X

//non-specific EOI command: clears interrupt request with highest priority that is currently active on any interrupt line, regardless of its level
#define OCW2_COMMAND_EOI OCW2_COMMAND(_OCW2_COMMAND_EOI)

//SPECIFIC eoi: clears the interrupt request with highest priority on the interrupt line specified in bits 2-0
#define OCW2_COMMAND_SEOI OCW2_COMMAND(_OCW2_COMMAND_SL|_OCW2_COMMAND_EOI)

//clear interrupt request with highest priority that is currently active on any interrupt line, and rotates the interrupt request levels
#define OCW2_COMMAND_ROTATION_EOI OCW2_COMMAND(_OCW2_COMMAND_R|_OCW2_COMMAND_EOI)

//start the AEOI mode and rotates the interrupt request levels
#define OCW2_COMMAND_SET_ROTAION_AEOI OCW2_COMMAND(_OCW2_COMMAND_R)

//clear the AEOI mode and rotate the interrupt priority levl
#define OCW2_COMMAND_CLEAR_ROTAION_AEOI OCW2_COMMAND(0b000)

//clear interrupt request with highest priority on the line specified in bits 2-0 (IR level), and then rotates the interrupt request levels.
#define OCW2_COMMAND_SET_ROTAION_AEOI OCW2_COMMAND(_OCW2_COMMAND_R|_OCW2_COMMAND_SL|_OCW2_COMMAND_EOI)

//Sets the priority of the interrupt line specified in bits 2-0 (IR level) to the highest level, and then rotates the interrupt request levels
#define OCW2_COMMAND_SET_PRIORITY OCW2_COMMAND(_OCW2_COMMAND_R|_OCW2_COMMAND_SL)

//no operation
#define OCW2_COMMAND_NOP OCW2_COMMAND(0b010)

//ENABLE SPECIAL MASK MODE
//IF ESMM==0, SMM WILL BE IGNORED
#define OCW3_ESMM (1<<6)
//SPECIAL MASK MODE
//IF ESMM==1:
//  SMM==0: REVERT TO NORMAL MASK MODE
//  SMM==1: ENTER SPECIAL MASK MODE
//ESMM==0: SMM IS A "DON'T CARE" VALUE
#define OCW3_SMM (1<<5)
//THE #3 BIT MUST BE SET FOR A VALID OCW3
#define VALID_OCW3 (1<<3)
//QUERY THE ISR OR IRR
#define OCW3_POLL (1<<2)

#define OCW3_POLL_IRR (0b10)
#define OCW3_POLL_ISR (0b11)

/*
Isr vs Irr
definition from internet:
ISR (In-Service Register): It is a register in the 8259A that indicates which interrupt request lines are currently being serviced by the CPU. When an interrupt request is received, the 8259A sets the corresponding bit in the In-Service Register to indicate that the CPU is currently servicing that interrupt. This prevents other interrupts with lower priority from being serviced until the current one has been completed.
IRR (Interrupt Request Register): It is another register in the 8259A that stores the status of the interrupt request lines. Each bit in the Interrupt Request Register corresponds to an interrupt request line, and when an interrupt request is received, the corresponding bit in the Interrupt Request Register is set. The 8259A uses this register to determine the highest-priority interrupt request that needs to be serviced by the CPU.

When an interrupt request is received, the 8259A checks the Interrupt Request Register to determine the highest-priority interrupt request that needs to be serviced. The 8259A then sets the corresponding bit in the In-Service Register and sends an interrupt signal to the CPU. Once the CPU has finished servicing the interrupt, it sends an End of Interrupt (EOI) signal to the 8259A, which clears the corresponding bit in the In-Service Register and checks the Interrupt Request Register again to determine the next interrupt to be serviced. This process continues until there are no more pending interrupt requests.

my own words:
the ISR is the current status, and the IRR is the incoming request, when the in-service routine is finished, the 8259A will check wether the IRR is empty, if so, a EOI signal will be fired;if not, 8259A will determine the priority, and issue a interrupt in IRR (both of them use bitmap to indicate which IR PIN has interrupt coming)


*/

/*
normal masking mode vs special masking mode
definition 1 from the internet:
The normal mask mode means that when masking an interrupt request, only the corresponding bit needs to be set in the interrupt mask register (IMR.in 8259A, OCW1 acted as IMR). Once the mode is enabled, the correspond interrupt request will be ignored and the interrupt will not be responded.
The special mask mode is a special interrupt mask mode, which can dynamically mask the interrupt request during the interrupt response process. In the special masking mode, when an interrupt request is responded, the interrupt controller will automatically mask the same or lower priority interrupt requests, and these interrupt requests will be cached in the special register of the interrupt controller until the end of the interrupt service routine And send a specific command before it will be responded. This mode is suitable for scenarios where low-priority interrupts need to be masked while high-priority interrupts are being processed.
The special mask mode is only valid for high-priority interrupts, and has no effect on low-priority interrupts

defintion 2 from the internet:
In the normal mask mode, when the CPU receives an interrupt and its interrupt mask bit (IMR) does not enable the interrupt line, the 8259A will notify the CPU of the incoming interrupt. CPU then sends a signal to the 8259A, telling it to handle the interrupt now. The 8259A then checks the interrupt mask bits again to see if there are other interrupts with higher priority that need to be handled, if so, inform the CPU of the higher priority interrupt and run the process again (check->execute). If not, action will not be taken
The special mask mode is similar to the normal mask mode, the difference is that when the CPU handles an interrupt, the 8259A will not only check the interrupt mask bit, but also check the ISR (interrupt status register) and IRR (interrupt request register) registers to see if There are other higher priority interrupts that need to be serviced. If so, the 8259A will tell the CPU to handle the next interrupt as soon as it finishes handling the current interrupt, rather than waiting for the CPU to send a command to the 8259A telling it interrupt handling is done. This can speed up interrupt processing and improve system efficiency.

in my own words:
normal masking mode requires the cpu to send a end of interrupt signal while 8259A working special masking mode will automatically check whether there are interrups coming (check the ISR and IRR register)

*/



//8259A pin
/*
8259A chipset
    8259A master
        PIN IR0: timer clock
        PIN IR1: keyboard
        PIN IR2: cascaded 8259A
        PIN IR3: serial port 2
        PIN IR4: serial port 1
        PIN IR5: parellel port 2
        PIN IR6: floppy dirver
        PIN IR7: parellel port 1
    8259A slave
        PIN IR0: CMOS RTC real-time clock
        PIN IR1: (connected to master 8259A)
        PIN IR2: RESERVED
        PIN IR3: RESERVED
        PIN IR4: PS/2 mouse
        PIN IR5: coprocessor
        PIN IR6: SATA master
        PIN IR7: SATA slave

*/
#define MASTER_BASE_IRQ 0x20
#define SLAVE_BASE_IRQ 0x28

#define TIMER_IRQ MASTER_BASE_IRQ+0
#define KEYBOARD_IRQ MASTER_BASE_IRQ+1
#define SLAVE_8259A_IRQ MASTER_BASE_IRQ+2
#define SERIAL2_IRQ MASTER_BASE_IRQ+3
#define SERIAL1_IRQ MASTER_BASE_IRQ+4
#define PARELLEL2_IRQ MASTER_BASE_IRQ+5
#define FLOPPY_IRQ MASTER_BASE_IRQ+6
#define PARELLEL1_IRQ MASTER_BASE_IRQ+7

#define CMOS_RTC_IRQ SLAVE_BASE_IRQ+0
#define MASTER_8259A_IRQ SLAVE_8259A_IRQ+1
#define RESERVED1_IRQ SLAVE_8259A_IRQ+2
#define RESERVED2_IRQ SLAVE_8259A_IRQ+3
#define PS2_MOUSE_IRQ SLAVE_8259A_IRQ+4
#define COPROCESSOR_IRQ SLAVE_8259A_IRQ+5
#define SATA_MASTER_IRQ SLAVE_8259A_IRQ+6
#define SATA_SLAVE_IRQ SLAVE_8259A_IRQ+7




#endif