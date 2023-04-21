#include "fonts/colors.h"
#include "printf.h"
#include "stdlib.h"
#include "string.h"
void Start_Kernel(void) {
    printf("kernel started, function %s running\n", __FUNCTION__);
    while (1) {
        ;
    }
}