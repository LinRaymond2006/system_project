#include "fonts/colors.h"
#include "printf.h"
#include "stdlib.h"
#include "string.h"
void Start_Kernel(void) {
    intit_sysirq();
    printf("kernel started, function %s running\n", __FUNCTION__);
    int i=1;
    int b=i/0;
    while (1) {
        ;
    }
}