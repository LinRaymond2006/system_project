//if the graphic card doesn't support mode 0x180, then the log won't be displayed
#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>
#include "string.h"
#include "stdlib.h"
#include "fonts/vgabios_font.h"
#include "fonts/colors.h"

typedef unsigned long size_t;

//1440/8=180
#define VBE_180H_TEXT_X 180
//900/16=56.25
#define VBE_180H_TEXT_Y 56
unsigned char text_buf[VBE_180H_TEXT_X * VBE_180H_TEXT_Y]={0};


typedef struct {
	unsigned int text_resX;
	unsigned int text_resY;
	unsigned char *buffer;
    unsigned int t_posX;
    unsigned int t_posY;
	unsigned int default_front;
	unsigned int default_back;
} CURSOR;

CURSOR csr = {
	.text_resX=VBE_180H_TEXT_X, 
	.text_resY=VBE_180H_TEXT_Y, 
	.buffer=(unsigned char *)&text_buf, 
	.t_posX=0, 
	.t_posY=0, 
	.default_front=COLOR_WHITE, 
	.default_back=COLOR_BLACK
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
unsigned char *default_font = (unsigned char *)&VGABIOS_font;

//will not update the cursor automatically
void putchar(unsigned char ascii) {
	//screen->cursor->posX=(screen->cursor->t_posX*FONT_COLS);
	//screen->cursor->posY=(screen->cursor->t_posY*FONT_ROWS);
	int rows, cols, bitmask;
	unsigned int *pixel_ptr;
	unsigned char *line_ptr=default_font+((int)ascii*16);
	unsigned int d_front=screen->cursor->default_front, d_back=screen->cursor->default_back;
	for(rows=0;rows < FONT_ROWS;rows++) {
		//increment of int pointer was in size of 4
		pixel_ptr=screen->frame_buffer + screen->resX*((screen->cursor->t_posY*FONT_ROWS)+rows)+(screen->cursor->t_posX*FONT_COLS);
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
	screen->cursor->t_posX++;
	if (screen->cursor->t_posX > screen->cursor->text_resX) {	//new line is required
		screen->cursor->t_posX = 0;
		if (screen->cursor->t_posY==screen->cursor->text_resY) {	//page need to be rolled
			;	//replace it with real page rolling function later
			screen->cursor->t_posY--;
		} else {
			screen->cursor->t_posY++;
		}
	}
	return;
}

void putstr(char *buffer, unsigned int length) {
	unsigned int i;
	for (i=0;i<length;i++) {
		putchar(buffer[i]);
		
	}
	return;
}

void scroll_line(int n) {

	memmove_int(screen->frame_buffer,	\
	(screen->frame_buffer+(screen->resX*FONT_ROWS*n)),		\
	((screen->resX*((screen->resY)-(n*FONT_ROWS)))));

	memset_int(screen->frame_buffer+(screen->resX*(screen->resY-(FONT_ROWS*n))),		\
	screen->cursor->default_back,		\
	screen->resX*(n*FONT_ROWS));

	screen->cursor->t_posY -= n;
	return;
}

//use putchar as downsteam interface
//maybe try to support ANSI color escape character later (if front and back are both 0)
//the default font color will be screen->cursor->default_[front/back]
//parser of variadic variable will be imple in the function too
void printf(const char* format, ...) {
	//char and short will be "promoted" to int in variadic arguments
    va_list arg_list;
    va_start(arg_list, format);

    char buffer[100];
    char *str_arg;
    int int_arg;
    char char_arg;
    unsigned int uint_arg;
    void *ptr_arg;
    const char *p;

    for (p = format; *p != '\0'; p++) {
        if (*p == '%') {
            switch (*(++p)) {
                case 's':
                    str_arg = va_arg(arg_list, char *);
                    putstr(str_arg, strlen(str_arg));
                    break;
                case 'd':
                    int_arg = va_arg(arg_list, int);
                    itoa(int_arg, buffer, 10);
                    putstr(buffer, strlen(buffer));
                    break;
                case 'c':
                    int_arg = va_arg(arg_list, int); // use int_arg instead of char_arg
                    char_arg = (char) int_arg; // convert int_arg back to char
                    putchar(char_arg);
                    break;
                case 'u':
                    uint_arg = va_arg(arg_list, unsigned int);
                    itoa(uint_arg, buffer, 10);
                    putstr(buffer, strlen(buffer));
                    break;
                case 'x':
                    uint_arg = va_arg(arg_list, unsigned int);
                    itoa(uint_arg, buffer, 16);
                    putstr(buffer, strlen(buffer));
                    break;
                case 'p':
					putchar('0');
					putchar('x');
                    ptr_arg = va_arg(arg_list, void *);
                    itoa((unsigned int)ptr_arg, buffer, 16);
                    putstr(buffer, strlen(buffer));
                    break;
                case '%':
                    putchar('%');
                    break;
                default:
                    break;
            }
        } else if (*p=='\n') {
			screen->cursor->t_posX=0;
			if (screen->cursor->t_posY > screen->cursor->text_resY-1) {
				;
				//roll line
			} else {
				screen->cursor->t_posY++;
			}
		} else if (*p=='\r') {
			screen->cursor->t_posX=0;
		} else {
            putchar(*p);
        }
    }

    va_end(arg_list);
	return;
}


void change_color(int new_front, int new_back, int update_profile) {
	int *pixel=screen->frame_buffer;
	int old_front=screen->cursor->default_front;
	int old_back=screen->cursor->default_back;

	int i;
	int max_pix = screen->resX*screen->resY;
	for (i=0;i<max_pix;i++) {
		if (pixel[i]==old_back) {
			pixel[i]=new_back;
		} else if (pixel[i]==old_front){
			pixel[i]=new_front;
		} else {
			;
		}
	}
	if (update_profile) {
		screen->cursor->default_front=new_front;
		screen->cursor->default_back=new_back;
	}
	return;
}