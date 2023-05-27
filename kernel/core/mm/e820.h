#ifndef __E820_H__
#define __E820_H__

struct e820_structure {
    unsigned long base;
    unsigned long length;
    unsigned int type;
} __attribute__((packed));

struct StructMemDesc {
    struct e820_structure E820Info[32] __attribute__((packed));
    unsigned long UsableMemory;
} __attribute__((packed));

extern void GetE820Struct();
extern struct StructMemDesc MemoryDesc;

#endif