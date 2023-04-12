//if the graphic card doesn't support mode 0x180, then the log won't be displayed
#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>
#include "fonts/vgabios_font.h"
#include "fonts/colors.h"


typedef struct {
	int posX;
	int posY;
	unsigned int defualt_front;
	unsigned int defualt_back;
} CURSOR;

CURSOR csr = {
	.posX=0, 
	.posY=0, 
	.defualt_front=COLOR_WHITE, 
	.defualt_back=COLOR_BLACK
};

typedef struct {
	int resX;
	int resY;
	int vbe_mode;
	unsigned int *frame_buffer;		//both virtual and physical address are accepted as long as it is in x86_64 canoncial address format

	CURSOR *cursor;
} DISPALY_DESCRIPTOR;

DISPALY_DESCRIPTOR scrn = {
	.resX=1440, 
	.resY=900, 
	.vbe_mode=0x180, 
	.frame_buffer=(unsigned int *)0xffff800000a00000, 
	.cursor=&csr, 
};

DISPALY_DESCRIPTOR *screen = &scrn;

//the font isn't hardcoded into the file, instead, it is pointed by a pointer, so any 2d array holding 256 unsinged chars are valid, giving programmer more flexibility
unsigned char *defualt_font = (unsigned char *)&VGABIOS_font;

void putchar(unsigned char ascii) {
	int rows, cols, bitmask;
	unsigned int *pixel_ptr;
	unsigned char *line_ptr=defualt_font+((int)ascii*16);
	unsigned int d_front=screen->cursor->defualt_front, d_back=screen->cursor->defualt_back;
	for(rows=0;rows < FONT_ROWS;rows++) {
		//increment of int pointer was in size of 4
		pixel_ptr=screen->frame_buffer + screen->resX*(screen->cursor->posY+rows)+screen->cursor->posX;
		bitmask=0b10000000;
		for(cols=0;cols < FONT_COLS;cols++) {
			if(*line_ptr & bitmask) {
				*pixel_ptr=d_front;
			} else {
				*pixel_ptr=d_back;
			}
			pixel_ptr++;
			bitmask >>= 1;
		}
		line_ptr++;		
	}
	return;
}

//use putchar as downsteam interface
//maybe try to support ANSI color escape character later (if front and back are both 0)
//the defualt font color will be screen->cursor->defualt_[front/back]
int printf(char *fmt_str, ...) {
	;
}