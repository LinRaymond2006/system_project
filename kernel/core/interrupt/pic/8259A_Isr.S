//set by host system--------
.set SREG_ES, 0x00
.set SREG_DS, 0x08
.set REG_R15, 0x10
.set REG_R14, 0x18
.set REG_R13, 0x20
.set REG_R12, 0x28
.set REG_R11, 0x30
.set REG_R10, 0x38
.set REG_R9, 0x40
.set REG_R8, 0x48
.set REG_RBP, 0x50
.set REG_RDI, 0x58
.set REG_RSI, 0x60
.set REG_RDX, 0x68
.set REG_RCX, 0x70
.set REG_RBX, 0x78
.set REG_RAX, 0x80
.set HANDLER_ADDR, 0x88
//--------------------------

//set by processor----------
.set ERRCODE, 0x90
.set REG_RIP, 0x98
.set SREG_CS, 0xa0
.set CREG_RFLAGS, 0xa8
.set REG_RSP_OLD, 0xb1
.set SREG_SS_OLD, 0xb8
//--------------------------



PIC8259A_IsrLauncher:
	cli

	cld
    pushq %rax      //real eax
	pushq %rbx
	pushq %rcx
	pushq %rdx
	pushq %rsi
	pushq %rdi
	pushq %rbp
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
	//pushq %ds is invalid
	movq %ds, %rax
	pushq %rax
	//pushq %es is invalid
	movq %es, %rax
	pushq %rax
	
	//switch to kernel 64 bit data segment
	movq $0x10, %rdi
	movq %rdi, %ds
	movq %rdi, %es


	//system V calling convention
	movq %rsp, %rdi             //register set pointer
    movq ERRCODE(%rsp), %rsi    //#NR value
	movq HANDLER_ADDR(%rsp), %rdx
	

	callq *%rdx

	//popq %es is invalid
	popq %rax
	movq %rax, %es
	//popq %ds is invalid
	popq %rax
	movq %rax, %ds

	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rbp
	popq %rdi
	popq %rsi
	popq %rdx
	popq %rcx
	popq %rbx
	popq %rax


	//balance the stack (error code, handler)
	//other stuff are maintain by the processor
	addq $(0x8+0x8), %rsp

	sti
	iretq

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

/*
code explanation:
    pushq $0                                //dummy error code (padding), pass the #NR to the program since error code field is not used. NOTE: push constant, do not ultilize any register else
    pushq %rax                              //dummy handler (padding), store the old rax
    leaq PicHandlerGeneral(%rip), %rax		//load the address of handler into rax register
    xchgq %rax, (%rsp)                      //the error handler is filled with correct value, and the old rax is back!
    jmp PIC8259A_IsrLauncher                //Launch the handler with all the value saved 
*/

.extern PicHandlerGeneral
.extern Ps2KeyboardHandler
//master
.global PIC_8259A_Timer_handler
PIC_8259A_Timer_handler:
    iretq
    pushq $0x0
    pushq %rax
    leaq PicHandlerGeneral(%rip), %rax
    xchgq %rax, (%rsp)
    jmp PIC8259A_IsrLauncher

.global PIC_8259A_Keyboard_handler
PIC_8259A_Keyboard_handler:
    pushq $0x1
    pushq %rax
    //leaq PicHandlerGeneral(%rip), %rax
    leaq Ps2KeyboardHandler(%rip), %rax
    xchgq %rax, (%rsp)
    jmp PIC8259A_IsrLauncher

.global PIC_8259A_Slave_handler
PIC_8259A_Slave_handler:
    pushq $0x2
    pushq %rax
    leaq PicHandlerGeneral(%rip), %rax
    xchgq %rax, (%rsp)
    jmp PIC8259A_IsrLauncher

.global PIC_8259A_SerialPort2_handler
PIC_8259A_SerialPort2_handler:
    pushq $0x3
    pushq %rax
    leaq PicHandlerGeneral(%rip), %rax
    xchgq %rax, (%rsp)
    jmp PIC8259A_IsrLauncher
    
.global PIC_8259A_SerialPort1_handler
PIC_8259A_SerialPort1_handler:
    pushq $0x4
    pushq %rax
    leaq PicHandlerGeneral(%rip), %rax
    xchgq %rax, (%rsp)
    jmp PIC8259A_IsrLauncher
    
.global PIC_8259A_ParellelPort2_handler
PIC_8259A_ParellelPort2_handler:
    pushq $0x5
    pushq %rax
    leaq PicHandlerGeneral(%rip), %rax
    xchgq %rax, (%rsp)
    jmp PIC8259A_IsrLauncher
    
.global PIC_8259A_Floppy_handler
PIC_8259A_Floppy_handler:
    pushq $0x6
    pushq %rax
    leaq PicHandlerGeneral(%rip), %rax
    xchgq %rax, (%rsp)
    jmp PIC8259A_IsrLauncher
    
.global PIC_8259A_ParellelPort1_handler
PIC_8259A_ParellelPort1_handler:
    pushq $0x7
    pushq %rax
    leaq PicHandlerGeneral(%rip), %rax
    xchgq %rax, (%rsp)
    jmp PIC8259A_IsrLauncher
    
//slave

.global PIC_8259A_CmosRtc_handler
PIC_8259A_CmosRtc_handler:
    pushq $0x8
    pushq %rax
    leaq PicHandlerGeneral(%rip), %rax
    xchgq %rax, (%rsp)
    jmp PIC8259A_IsrLauncher
    
.global PIC_8259A_Master_handler
PIC_8259A_Master_handler:
    pushq $0x9
    pushq %rax
    leaq PicHandlerGeneral(%rip), %rax
    xchgq %rax, (%rsp)
    jmp PIC8259A_IsrLauncher
    
.global PIC_8259A_Reserved1_handler
PIC_8259A_Reserved1_handler:
    pushq $0xa
    pushq %rax
    leaq PicHandlerGeneral(%rip), %rax
    xchgq %rax, (%rsp)
    jmp PIC8259A_IsrLauncher
    
.global PIC_8259A_Reserved2_handler
PIC_8259A_Reserved2_handler:
    pushq $0xb
    pushq %rax
    leaq PicHandlerGeneral(%rip), %rax
    xchgq %rax, (%rsp)
    jmp PIC8259A_IsrLauncher
    
.global PIC_8259A_Ps2Mouse_handler
PIC_8259A_Ps2Mouse_handler:
    pushq $0xc
    pushq %rax
    leaq PicHandlerGeneral(%rip), %rax
    xchgq %rax, (%rsp)
    jmp PIC8259A_IsrLauncher
    
.global PIC_8259A_Coprocessor_handler
PIC_8259A_Coprocessor_handler:
    pushq $0xd
    pushq %rax
    leaq PicHandlerGeneral(%rip), %rax
    xchgq %rax, (%rsp)
    jmp PIC8259A_IsrLauncher
    
.global PIC_8259A_SataMaster_handler
PIC_8259A_SataMaster_handler:
    pushq $0xe
    pushq %rax
    leaq PicHandlerGeneral(%rip), %rax
    xchgq %rax, (%rsp)
    jmp PIC8259A_IsrLauncher
    
.global PIC_8259A_SataSlave_handler
PIC_8259A_SataSlave_handler:
    pushq $0xf
    pushq %rax
    leaq PicHandlerGeneral(%rip), %rax
    xchgq %rax, (%rsp)
    jmp PIC8259A_IsrLauncher
