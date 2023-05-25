struct e820_structure {
	unsigned long base;
	unsigned long length;
	unsigned int type;
} __attribute__((packed));

struct e820_structure memstruct[32];
//struct e820_structure (*memstruct)[32]=&;


//MEMORY INFORMAION AT 0x7e00

#define MEMTYPE_AVAILABLE 1
//reserved memory, ROM, etc
#define MEMTYPE_RESERVE   2
#define MEMTYPE_ACPI      3
#define MEMTYPE_ACPINVS   4

extern unsigned long ParseE820Struct(struct e820_structure (*e820_ptr)[32]) {
    int i;
    unsigned long AvailableMemory=0;
    for (i=0;i<32;i++) {
        printf("memory region found:    ");
        switch (e820_ptr[i]->type) {
            case MEMTYPE_AVAILABLE:
                printf("base: %p, length: %p, type: 1 (available memory)\n", e820_ptr[i]->base, e820_ptr[i]->length);
                AvailableMemory+=e820_ptr[i]->length;
                break;
            case MEMTYPE_RESERVE:
                printf("base: %p, length: %p, type: 2 (reserved memory)\n", e820_ptr[i]->base, e820_ptr[i]->length);
                break;
            case MEMTYPE_ACPI:
                printf("base: %p, length: %p, type: 3 (ACPI memory)\n", e820_ptr[i]->base, e820_ptr[i]->length);
                break;
            case MEMTYPE_ACPINVS:
                printf("base: %p, length: %p, type: 4 (APCI Non-Volatile Storage)\n", e820_ptr[i]->base, e820_ptr[i]->length);
                break;
            default:
                printf("(invalid entry)\n");
        }
    }
    printf("usable memory in total: %p\n", AvailableMemory);
}