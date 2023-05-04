#include "printf.h"
#include "stdlib.h"
#include "string.h"

//extern void set_trap_gate();
extern void *IDT_POINTER;
extern inline void set_idt_entry();
extern void set_tsstable();
void aaa();

void Start_Kernel(void) {
    printf("kernel started, function %s running\n", __FUNCTION__);
    set_tsstable(0xffff800000007c00, 0xffff800000007c00, 0xffff800000007c00, 0xffff800000007c00, 0xffff800000007c00, 0xffff800000007c00, 0xffff800000007c00, 0xffff800000007c00, 0xffff800000007c00, 0xffff800000007c00);
    set_idt_entry(0x0, (unsigned long)&aaa, 0x08, 0, 0xf, 0);
    set_idt_entry(0x80, (unsigned long)&aaa, 0x08, 0, 0xE, 0);
    __asm__ __volatile__ ("lidt %0": : "m"(IDT_POINTER):);
    //__asm__ __volatile__ ("int $0x80":::);
    int i=1;
    int b=i/0;
    while (1) {
        ;
    }
    return;
}

void aaa() {
    printf("\n%s invoked\n", __PRETTY_FUNCTION__);
    while(1) ;
}