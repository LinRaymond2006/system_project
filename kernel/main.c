#include "screen/printf.h"
#include "screen/fonts/colors.h"
void Start_Kernel(void) {
    putchar('A', COLOR_YELLOWGREEN, 0);
    while (1) {
        ;
    }
}
