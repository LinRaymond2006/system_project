#ifndef __GATE_H__
#define __GATE_H__

#define NO_IST 0
//used for software inturrupt
#define GATE_INT 0xe
//used for hardware trap
#define GATE_TRAP 0xf
#define DPL_SYS 0
#define DPL_USR 3

extern inline void set_ivte(void *ivt_base, int irq, void *target, int seg_selector, int ist, int type, int dpl) {
    unsigned long val0,val1;
    val0=(unsigned long)target>>32;
    val1=((unsigned long)target&0xffffffff0000ffff)>>16;
    val1|=((unsigned long)target&0xffffffffffff0000);
    val1|=(unsigned long)seg_selector<<16;
    val1|=(unsigned long)ist<<32;
    val1|=(unsigned long)type<<40;
    val1|=(unsigned long)dpl<<45;
    val1|=0b100000000000000000000000000000000000000000000000;            //present bit, set if the entry is valid
    *((unsigned long *)(ivt_base + irq*2)) = val0;
    *((unsigned long *)(ivt_base + (irq*2)+1)) = val1;
    return;
}



#endif