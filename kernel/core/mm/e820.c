#include "string.h"
#include "printf.h"
/*
struct StructMemDesc {
    struct e820_structure {
        unsigned long base;
        unsigned long length;
        unsigned int type;
    } E820Info[32] __attribute__((packed));
    unsigned long UsableMemory;
} MemoryDesc __attribute__((packed));

//struct e820_structure (*memstruct)[32]=&;


//MEMORY INFORMAION AT 0x7e00

#define MEMTYPE_AVAILABLE 1
//reserved memory, ROM, etc
#define MEMTYPE_RESERVE   2
#define MEMTYPE_ACPI      3
#define MEMTYPE_ACPINVS   4

void SaveMemoryInfo(struct e820_structure *src, struct e820_structure *dest) {
    memcpy((void *)dest, (void *)src, sizeof(struct e820_structure)*32);
}
unsigned long ParseE820Struct(struct StructMemDesc *TargetMemDesc) {
    int i;
    unsigned long AvailableMemory=0;
    for (i=0;i<32;i++) {
        printf("memory region found:    ");
        switch (TargetMemDesc->E820Info[i].type) {
            case MEMTYPE_AVAILABLE:
                printf("base: %p, length: %p, type: 1 (available memory)\n", TargetMemDesc->E820Info[i].base, TargetMemDesc->E820Info[i].length);
                AvailableMemory+=TargetMemDesc->E820Info[i].length;
                break;
            case MEMTYPE_RESERVE:
                printf("base: %p, length: %p, type: 2 (reserved memory)\n", TargetMemDesc->E820Info[i].base, TargetMemDesc->E820Info[i].length);
                break;
            case MEMTYPE_ACPI:
                printf("base: %p, length: %p, type: 3 (ACPI memory)\n", TargetMemDesc->E820Info[i].base, TargetMemDesc->E820Info[i].length);
                break;
            case MEMTYPE_ACPINVS:
                printf("base: %p, length: %p, type: 4 (APCI Non-Volatile Storage)\n", TargetMemDesc->E820Info[i].base, TargetMemDesc->E820Info[i].length);
                break;
            default:
                printf("(invalid entry) base: %p, length: %p, type: %d\n", TargetMemDesc->E820Info[i].base, TargetMemDesc->E820Info[i].length, TargetMemDesc->E820Info[i].type);
        }
    }
    printf("usable memory in total: %p\n", AvailableMemory);
    return AvailableMemory;
}




extern void GetE820Struct(struct e820_structure *dest_ptr) {
    SaveMemoryInfo((void *)0xba00, (void *)&MemoryDesc.E820Info[0]);
    MemoryDesc.UsableMemory=ParseE820Struct(&MemoryDesc);
    
    //ParseE820Struct((void *)0xba00);
}

*/

#define MEMTYPE_AVAILABLE 1
#define MEMTYPE_RESERVE   2
#define MEMTYPE_ACPI      3
#define MEMTYPE_ACPINVS   4

struct e820_structure {
    unsigned long base;
    unsigned long length;
    unsigned int type;
} __attribute__((packed));

struct StructMemDesc {
    struct e820_structure E820Info[32] __attribute__((packed));
    unsigned long UsableMemory;
} __attribute__((packed));

struct StructMemDesc MemoryDesc;

void SaveMemoryInfo(struct e820_structure *src, struct e820_structure *dest) {
    memcpy(dest, src, sizeof(struct e820_structure) * 32);
}

unsigned long ParseE820Struct(struct StructMemDesc *TargetMemDesc) {
    int i;
    unsigned long AvailableMemory = 0;
    for (i = 0; i < 32; i++) {
        printf("memory region found:    ");
        switch (TargetMemDesc->E820Info[i].type) {
            case MEMTYPE_AVAILABLE:
                printf("base: %p, length: %p, type: 1 (available memory)\n",
                    TargetMemDesc->E820Info[i].base,
                    TargetMemDesc->E820Info[i].length);
                AvailableMemory += TargetMemDesc->E820Info[i].length;
                break;
            case MEMTYPE_RESERVE:
                printf("base: %p, length: %p, type: 2 (reserved memory)\n",
                    TargetMemDesc->E820Info[i].base,
                    TargetMemDesc->E820Info[i].length);
                break;
            case MEMTYPE_ACPI:
                printf("base: %p, length: %p, type: 3 (ACPI memory)\n",
                    TargetMemDesc->E820Info[i].base,
                    TargetMemDesc->E820Info[i].length);
                break;
            case MEMTYPE_ACPINVS:
                printf("base: %p, length: %p, type: 4 (ACPI Non-Volatile Storage)\n",
                    TargetMemDesc->E820Info[i].base,
                    TargetMemDesc->E820Info[i].length);
                break;
            default:
                printf("base: %p, length: %p, type: %p (*INVALID ENTRY*)\n",
                    TargetMemDesc->E820Info[i].base,
                    TargetMemDesc->E820Info[i].length,
                    TargetMemDesc->E820Info[i].type);
        }
    }
    printf("usable memory in total: %p\n", AvailableMemory);
    return AvailableMemory;
}

extern void GetE820Struct(struct e820_structure *dest_ptr) {
    SaveMemoryInfo((void *)0xba00, &MemoryDesc.E820Info[0]);
    MemoryDesc.UsableMemory = ParseE820Struct(&MemoryDesc);
    return;
}
