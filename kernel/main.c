#include "printf.h"
#include "stdlib.h"
#include "string.h"
#include "interrupt/exception/error_handler.h"
#include "interrupt/pic/8259A_handler.h"

void Start_Kernel(void) {
    printf("kernel started, function %s running\n", __FUNCTION__);
    RegisterHandlerIrq();
    Register8259AIrq();
    //int i=1/0;

    __asm__ __volatile__ ("jmp .":::);
    /*while (1) {
        ;
    }*/
    return;
}
