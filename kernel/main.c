#include "printf.h"
#include "stdlib.h"
#include "string.h"
#include "interrupt/exception/error_handler.h"

void Start_Kernel(void) {
    printf("kernel started, function %s running\n", __FUNCTION__);
    RegisterHandlerIrq();
    while (1) {
        ;
    }
    return;
}
