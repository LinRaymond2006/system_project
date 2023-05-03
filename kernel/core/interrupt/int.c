struct IDT_entry {
    unsigned short offset_low;     // offset bits 0..15
    unsigned short selector;       // a code segment selector in GDT or LDT
    unsigned char ist;             // bits 0..2 holds Interrupt Stack Table offset, rest of bits zero.
    unsigned char type_attr;       // type and attributes
    unsigned short offset_mid;     // offset bits 16..31
    unsigned int offset_high;    // offset bits 32..63
    unsigned int zero;           // reserved, must be zero
};

// IDT table
extern struct IDT_entry IDT_Table[];

// IDT pointer
extern void IDT_POINTER();

void set_idt_entry(unsigned short index, unsigned long handler_address, unsigned short selector, unsigned char dpl, unsigned char type, unsigned char ist) {
	//unregister it first
    struct IDT_entry entry={0};							//temp entry
	IDT_Table[index]=entry;
    entry.offset_low = handler_address & 0xFFFF;	//low address
    entry.selector = selector;						//selector number
    entry.ist = ist & 0x7;							//clear the reserved field using AND
    entry.type_attr = (dpl & 0x3) << 5 | (1 << 4) | type;	//typde filed
    entry.offset_mid = (handler_address >> 16) & 0xFFFF;	//address middle
    entry.offset_high = handler_address >> 32;				//address high
    entry.zero = 0;											//high reserved field
    IDT_Table[index] = entry;
}

//https://wiki.osdev.org/I_Can%27t_Get_Interrupts_Working#My_handler_doesn.27t_get_called.3F.21_.28Assembly.29
//https://wiki.osdev.org/IDT