#include "screen/printf.h"
void Start_Kernel(void) {
    init_vbe3_180h();
    putchar((int)'A', 0x00000000, 0x00ffffff);
    while (1) {
        ;
    }
}
