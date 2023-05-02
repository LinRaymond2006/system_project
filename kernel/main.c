#include "printf.h"
#include "stdlib.h"
#include "string.h"
void aaa() {
    while(1) ;
}

//extern void set_trap_gate();
extern void *IDT_POINTER;
extern inline void set_trap_gate();

void Start_Kernel(void) {
    
    //set_trap_gate((int)0,(char)1,aaa);
    //__asm__ __volatile__ ("lidt %0": : "m"(IDT_POINTER):);
    printf("kernel started, function %s running\n", __FUNCTION__);
    int i=1;
    int b=i/0;
    while (1) {
        ;
    }
}
