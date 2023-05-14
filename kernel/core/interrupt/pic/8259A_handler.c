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

extern void PIC_8259A_Timer_handler();
extern void PIC_8259A_Keyboard_handler();
extern void PIC_8259A_Slave_handler();
extern void PIC_8259A_SerialPort2_handler();
extern void PIC_8259A_SerialPort1_handler();
extern void PIC_8259A_ParellelPort2_handler();
extern void PIC_8259A_Floppy_handler();
extern void PIC_8259A_ParellelPort1_handler();
extern void PIC_8259A_CmosRtc_handler();
extern void PIC_8259A_Master_handler();
extern void PIC_8259A_Reserved1_handler();
extern void PIC_8259A_Reserved2_handler();
extern void PIC_8259A_Ps2Mouse_handler();
extern void PIC_8259A_Coprocessor_handler();
extern void PIC_8259A_SataMaster_handler();
extern void PIC_8259A_SataSlave_handler();



/*
extern void SetMasterIcw(char Irq, char Icw1, char PinMap, char Icw4);
extern void SetSlaveIcw(char Irq, char Icw1, char CascadedPin, char Icw4);
extern void ConfigMasterOcw(char Ocw1, char Ocw2, char Ocw3);
extern void ConfigSlaveOcw(char Ocw1, char Ocw2, char Ocw3);
*/

#define SYSTEM_INT_ENTRY(IRQ, TARGET, IST) SetIdtEntry(IRQ, TARGET, 0x8, 0, 0xe, IST)

extern void Register8259AIrq() {
	__asm__ __volatile__ ("cli":::"memory");
    printf("initializing 8259A controller\n");
    //8259A master icw

    SYSTEM_INT_ENTRY(TIMER_IRQ, PIC_8259A_Timer_handler, 1);
    SYSTEM_INT_ENTRY(KEYBOARD_IRQ, PIC_8259A_Keyboard_handler, 1);
    SYSTEM_INT_ENTRY(SLAVE_8259A_IRQ, PIC_8259A_Slave_handler, 1);
    SYSTEM_INT_ENTRY(SERIAL2_IRQ, PIC_8259A_SerialPort2_handler, 1);
    SYSTEM_INT_ENTRY(SERIAL1_IRQ, PIC_8259A_SerialPort1_handler, 1);
    SYSTEM_INT_ENTRY(PARELLEL2_IRQ, PIC_8259A_ParellelPort2_handler, 1);
    SYSTEM_INT_ENTRY(FLOPPY_IRQ, PIC_8259A_Floppy_handler, 1);
    SYSTEM_INT_ENTRY(PARELLEL1_IRQ, PIC_8259A_ParellelPort1_handler, 1);
    SYSTEM_INT_ENTRY(CMOS_RTC_IRQ, PIC_8259A_CmosRtc_handler, 1);
    SYSTEM_INT_ENTRY(MASTER_8259A_IRQ, PIC_8259A_Master_handler, 1);
    SYSTEM_INT_ENTRY(RESERVED1_IRQ, PIC_8259A_Reserved1_handler, 1);
    SYSTEM_INT_ENTRY(RESERVED2_IRQ, PIC_8259A_Reserved2_handler, 1);
    SYSTEM_INT_ENTRY(PS2_MOUSE_IRQ, PIC_8259A_Reserved2_handler, 1);
    SYSTEM_INT_ENTRY(COPROCESSOR_IRQ, PIC_8259A_Coprocessor_handler, 1);
    SYSTEM_INT_ENTRY(SATA_MASTER_IRQ, PIC_8259A_SataMaster_handler, 1);
    SYSTEM_INT_ENTRY(SATA_SLAVE_IRQ, PIC_8259A_SataSlave_handler, 1);


	SetMasterIcw(0x11, 0x20, 0x04, 0x01);

	//8259A slave icw
	SetSlaveIcw(0x11, 0x28, 0x02, 0x01);

	//8259A ocw1
	InitMasterOcw1(0xfd);
	InitSlaveOcw1(0xff);

    printf("done\n");
	printf("enableing PS/2 keyboard\n");
	EnableKeyboard();
	printf("done\n");
	__asm__ __volatile__ ("sti":::"memory");
    return;
}


extern void PicHandlerGeneral(unsigned long RegTablePtr, unsigned long nr) {
	//unsigned char scancode=in_b(0x60);
    printf("%s fired by 8259A (#NR: %p)\n", __PRETTY_FUNCTION__, nr);
	//not sure, do not modify for now
	out_b(PORT0_8259A_MASTER,0x20);
    return;
}

void EnableKeyboard(void){
	return;
}
//nothing is changed
static const char SCANCODE2CHAR[128] = {	\
  0,   0,   '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-',  '=',  '\b', '\t',	\
  'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0,    'a', 's',	\
  'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,   '\\', 'z', 'x',  'c', 'v',	\
  'b', 'n', 'm', ',', '.', '/', 0,    '*', 0,   ' ', 0,    0,   0,   0,    0,   0, 	\
  0,   '7', '8', '9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.',  0,   0, 	\
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,    0,   0,   0,    0,	\
   0,   0,   0,   0,   0,   0,   0,   0,   0,   '(', ')', '{',  '}',  '\n', 0,   0,	\
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,    0,   0,   0,    0	\
};
//everything is changed
static const char SHIFTED_SCANCODE2CHAR[128] = {	\
  0,   0,   '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_',  '+',  '\b', '\t',	\
  'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', 0,    'A', 'S',	\
  'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '\'', '`', 0,   '\\', 'Z', 'X',  'C', 'V',	\
  'B', 'N', 'M', '<', '>', '?', 0,    '*', 0,   ' ', 0,    0,   0,   0,    0,   0, 	\
  0,   '7', '8', '9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.',  0,   0, 	\
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,    0,   0,   0,    0,	\
   0,   0,   0,   0,   0,   0,   0,   0,   0,   '(', ')', '{',  '}',  '\n', 0,   0,	\
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,    0,   0,   0,    0	\
};
//only alphabet are not changed
static const char SHIFTED_CAPSLOCKED_SCANCODE2CHAR[128] = {	\
  0,   0,   '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_',  '+',  '\b', '\t',	\
  'q', 'w', 'e', 'r', 't', 'y', 'u', 'I', 'o', 'p', '{', '}', '\n', 0,    'a', 's',	\
  'd', 'f', 'g', 'h', 'j', 'k', 'l', ':', '\'', '`', 0,   '\\', 'z', 'x',  'c', 'v',	\
  'b', 'n', 'm', '<', '>', '?', 0,    '*', 0,   ' ', 0,    0,   0,   0,    0,   0, 	\
  0,   '7', '8', '9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.',  0,   0, 	\
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,    0,   0,   0,    0,	\
   0,   0,   0,   0,   0,   0,   0,   0,   0,   '(', ')', '{',  '}',  '\n', 0,   0,	\
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,    0,   0,   0,    0	\
};
//only alphabet are changed
static const char CAPSLOCKED_SCANCODE2CHAR[128] = {	\
  0,   0,   '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-',  '=',  '\b', '\t',	\
  'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', '\n', 0,    'A', 'S',	\
  'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '\'', '`', 0,   '\\', 'Z', 'X',  'C', 'V',	\
  'B', 'N', 'M', ',', '.', '/', 0,    '*', 0,   ' ', 0,    0,   0,   0,    0,   0, 	\
  0,   '7', '8', '9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.',  0,   0, 	\
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,    0,   0,   0,    0,	\
   0,   0,   0,   0,   0,   0,   0,   0,   0,   '(', ')', '{',  '}',  '\n', 0,   0,	\
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,    0,   0,   0,    0	\
};

char get_char_from_scan_code(char ScanCode) {
	static char caps_locked=0;
    static char shift_pressed = 0; // keep track of whether the shift key is pressed
    if (ScanCode & 0x80) { // break code
        char MakeCode = ScanCode & 0b1111111; // strip the high bit to get the make code
        if (MakeCode == 0x2a || MakeCode == 0x36) { // shift key released
            shift_pressed = 0;
        }
        return 0; // ignore break codes
    } else { // make code
        char MakeCode = ScanCode;
        if (MakeCode == 0x2a || MakeCode == 0x36) { // shift key pressed
            shift_pressed = 1;
            return 0;
        }
        if (MakeCode==0x3a) {
            caps_locked= !caps_locked;
        }
        char c = SCANCODE2CHAR[MakeCode];
        if ((shift_pressed==1) && (caps_locked==0)) { // shift key held down
            c = SHIFTED_SCANCODE2CHAR[MakeCode];
        } else if ((shift_pressed==1) && (caps_locked==1)) {
            c = SHIFTED_CAPSLOCKED_SCANCODE2CHAR[MakeCode];
        } else if ((shift_pressed==0) && (caps_locked==1)) {
            c = CAPSLOCKED_SCANCODE2CHAR[MakeCode];
        } else { //neither CAPS nor SHIFT is pressed
            c = SCANCODE2CHAR[MakeCode];
        }
        return c;
    }
}

extern void Ps2KeyboardHandler(unsigned long RegTablePtr, unsigned long nr) {
	unsigned char ScanCode=in_b(0x60);
	unsigned char Key = get_char_from_scan_code(ScanCode);
	if (Key) {
		printf("%c", Key);
	}

	send_ACK:
		out_b(PORT0_8259A_MASTER,0x20);
		return;
}