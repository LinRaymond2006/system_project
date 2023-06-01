#include "printf.h"
typedef struct {
    unsigned short offset_low;     // offset bits 0..15
    unsigned short selector;       // a code segment selector in GDT or LDT
    unsigned char ist;             // bits 0..2 holds Interrupt Stack Table offset, rest of bits zero.
    unsigned char type_attr;       // type and attributes
    unsigned short offset_mid;     // offset bits 16..31
    unsigned int offset_high;    // offset bits 32..63
    unsigned int zero;           // reserved, must be zero
} IDTE;

// IDT table
extern IDTE IDT_Table[255];

// IDT pointer
extern void *IDT_POINTER;

extern void SetIdtEntry(unsigned short index, unsigned long handler_address, unsigned short selector, unsigned char dpl, unsigned char type, unsigned char ist) {
    //printf("registering irq %d\n  target: %p\n  selector: %p\n  dpl: %p\n  type: %p\n  ist:%p\n", index, handler_address, selector, dpl, type, ist);
    IDTE entry={0};							//temp entry
    entry.offset_low = handler_address & 0xFFFF;	//low address
    entry.selector = selector;						//selector number
    entry.ist = ist & 0x7;							//clear the reserved field using AND
    entry.type_attr = (dpl & 0x3) << 5 | (1 << 7) | type;	//typde filed
    entry.offset_mid = (handler_address >> 16) & 0xFFFF;	//address middle
    entry.offset_high = handler_address >> 32;				//address high
    entry.zero = 0;											//high reserved field
    IDT_Table[index] = entry;
    //printf("done\n");
    return;
}


typedef struct {
    unsigned int RES1;
    unsigned long RSP0;
    unsigned long RSP1;
    unsigned long RSP2;
    unsigned long RES2;
    unsigned long IST1;
    unsigned long IST3;
    unsigned long IST2;
    unsigned long IST4;
    unsigned long IST5;
    unsigned long IST6;
    unsigned long IST7;
    unsigned long RES3;
    unsigned short RES4;
    unsigned short IOBP;
} __attribute__((packed)) StructTssTable;

extern StructTssTable TSS64_Table;
extern void SetTssTable(unsigned long rsp0,unsigned long rsp1,unsigned long rsp2,unsigned long ist1,unsigned long ist2, unsigned long ist3, unsigned long ist4, unsigned long ist5, unsigned long ist6, unsigned long ist7) {
     //printf("updateing TSS table:\n rsp0: %p rsp1: %p rsp2: %p ist1: %p \n ist2: %p ist3: %p ist4:> TSS64_Table[1]=rsp0 & 0xffffffff;


    TSS64_Table.IOBP=0;
    TSS64_Table.IST1=ist1;
    TSS64_Table.IST1=ist2;
    TSS64_Table.IST1=ist3;
    TSS64_Table.IST1=ist4;
    TSS64_Table.IST1=ist5;
    TSS64_Table.IST1=ist6;
    TSS64_Table.IST1=ist7;
    TSS64_Table.RSP0=rsp0;
    TSS64_Table.RSP0=rsp1;
    TSS64_Table.RSP0=rsp2;
return;
}
