extern void *IDT_POINTER;
extern void SetIdtEntry(unsigned short index, unsigned long handler_address, unsigned short selector, unsigned char dpl, unsigned char type, unsigned char ist);




//Isr_8259A_General
//interrupt: DPL0, 0XE
//trap: DPL0, 0XF
//system call: DPL3, 0XF
extern void *Isr_8259A_General;
extern void Register8259AIrq() {
    for (int i=0x20;i<0x10;i++) {
        SetIdtEntry(i, Isr_8259A_General, 0x8, 0, 0xe, 0);
    }
    return;
}


extern void PicHandlerGeneral(unsigned long RegTablePtr, unsigned long nr) {
    printf("%s just got invoked, stopping :P\n", __PRETTY_FUNCTION__);
    return;
}