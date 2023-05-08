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

void SetIdtEntry(unsigned short index, unsigned long handler_address, unsigned short selector, unsigned char dpl, unsigned char type, unsigned char ist) {
    IDTE entry={0};							//temp entry
    entry.offset_low = handler_address & 0xFFFF;	//low address
    entry.selector = selector;						//selector number
    entry.ist = ist & 0x7;							//clear the reserved field using AND
    entry.type_attr = (dpl & 0x3) << 5 | (1 << 7) | type;	//typde filed
    entry.offset_mid = (handler_address >> 16) & 0xFFFF;	//address middle
    entry.offset_high = handler_address >> 32;				//address high
    entry.zero = 0;											//high reserved field
    IDT_Table[index] = entry;
}


extern unsigned long *TSS64_Table;
extern inline void SetTssTable(unsigned long rsp0,unsigned long rsp1,unsigned long rsp2,unsigned long ist1,unsigned long ist2,unsigned long ist3,
unsigned long ist4,unsigned long ist5,unsigned long ist6,unsigned long ist7)
{
    TSS64_Table[1]=rsp0;
    TSS64_Table[3]=rsp1;
    TSS64_Table[5]=rsp2;
    TSS64_Table[9]=ist1;
    TSS64_Table[11]=ist2;
    TSS64_Table[13]=ist3;
    TSS64_Table[15]=ist4;
    TSS64_Table[17]=ist5;
    TSS64_Table[19]=ist6;
    TSS64_Table[21]=ist7;
    return;
}
