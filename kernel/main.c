#include "printf.h"
#include "stdlib.h"
#include "string.h"
#include "interrupt/exception/error_handler.h"

extern void init_interrupt();
void Start_Kernel(void) {
    printf("kernel started, function %s running\n", __FUNCTION__);
    RegisterHandlerIrq();
    init_interrupt();
    //int i=1/0;
    while (1) {
        ;
    }
    return;
}
