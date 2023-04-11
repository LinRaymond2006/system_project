//if the graphic card doesn't support mode 0x180, then the log won't be displayed

#include <stdarg.h>
#include <stdint.h>
#include "fonts/vgabios_font.h"
typedef struct {
    int posX;
    int posY;
    int maxX;
    int maxY;
    int *cur_ptr;
    int mode;
    int color_depth;
} SCREEN;
SCREEN scr, *screen=&scr;

#define MODE_0X180_FRAME_BASE 0xe0000000
#define FRAME_BASE 0xffff800000a00000
void setup_0x180() {
    screen->posX=0;
    screen->posY=0;
    screen->cur_screen=FRAME_BASE;

}

int *rel_addr(int x, int y) {
    //calcutated the physical address of the font buffer, 
    return (int *)(((y*16)*(1440)+(x*8))*(screen->color_depth));
}

void putchar(int ascii, int front, int back) {
    char *lines, val;
    int *ptr=rel_addr(screen->posX, screen->posY);
    int x, y;
    for (y=0;y<16;y++) {
        lines=&font[ascii][y];
        val= *lines;
        if (val & 0b00000001) {
            
        }
        if (val & 0b00000010) {

        }
        if (val & 0b00000100) {

        }
        if (val & 0b00001000) {

        }
        if (val & 0b00010000) {

        }
        if (val & 0b00100000) {

        }
        if (val & 0b01000000) {

        }
        if (val & 0b10000000) {

        }
        (char *)ptr+=1440;

    }
}