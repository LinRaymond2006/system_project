extern void *IDT_POINTER;
extern void SetIdtEntry(unsigned short index, unsigned long handler_address, unsigned short selector, unsigned char dpl, unsigned char type, unsigned char ist);
#include "printf.h"
#include "gpio.h"
#include "8259A_handler.h"
#include "8259A_cw.h"

//Isr_8259A_General
//interrupt: DPL0, 0XE
//trap: DPL0, 0XF
//system call: DPL3, 0XF
extern void *Isr_8259A_General;
/*
extern void SetMasterIcw(char Irq, char Icw1, char PinMap, char Icw4);
extern void SetSlaveIcw(char Irq, char Icw1, char CascadedPin, char Icw4);
extern void ConfigMasterOcw(char Ocw1, char Ocw2, char Ocw3);
extern void ConfigSlaveOcw(char Ocw1, char Ocw2, char Ocw3);
*/
extern void Register8259AIrq() {
    printf("initializing 8259A controller\n");
    //8259A master icw
	SetMasterIcw(0x11, 0x20, 0x04, 0x01);

	//8259A slave icw
	SetSlaveIcw(0x11, 0x28, 0x02, 0x01);

	//8259A ocw1
	InitMasterOcw1(0xfd);
	InitSlaveOcw1(0xff);

	printf("registering irq for pic\n");

    for (int i=0x20;i<0x30;i++) {
        SetIdtEntry(i, Isr_8259A_General, 0x8, 0, 0xe, 1);
    }
	__asm__ __volatile__ ("sti":::);
    printf("done\n");
    return;
}


extern void PicHandlerGeneral(unsigned long RegTablePtr, unsigned long nr) {
    printf("%s just got invoked\n", __PRETTY_FUNCTION__);
	//not sure, do not modify for now
	out_b(PORT0_8259A_MASTER,0x20);
    return;
}