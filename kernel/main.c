#include "printf.h"
#include "stdlib.h"
#include "string.h"
#include "interrupt/int.h"
#include "mm/memory.h"

void Start_Kernel(void) {
    printf("kernel started, function %s running\n", __FUNCTION__);
    RegisterHandlerIrq();
    Register8259AIrq();
    cli();
    GetE820Struct(&MemoryDesc);
    __asm__ __volatile__ (  \
        "int $0x31  \n\t"   \
        :::"memory"         \
    );

    __asm__ __volatile__ ("jmp .":::);
    return;
}
