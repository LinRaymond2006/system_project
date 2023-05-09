//if the graphic card doesn't support mode 0x180, then the log won't be displayed
#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>
#include "string.h"
#include "stdlib.h"
#include "fonts/vgabios_font.h"
#include "fonts/colors.h"
//test

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

void putchar(unsigned char ascii) {
	//screen->cursor->posX=(screen->cursor->t_posX*FONT_COLS);
	//screen->cursor->posY=(screen->cursor->t_posY*FONT_ROWS);
    int rows, cols, bitmask;
    unsigned int *pixel_ptr;
    unsigned char *line_ptr = default_font + ((int) ascii * 16);
    unsigned int d_front = screen->cursor->default_front, d_back = screen->cursor->default_back;

    // check if the current position is at the end of the line and move to the beginning of the next line if necessary
    if (screen->cursor->t_posX >= screen->cursor->text_resX) {
        screen->cursor->t_posX = 0;
        screen->cursor->t_posY++;
        if (screen->cursor->t_posY >= screen->cursor->text_resY) {
            // scroll the screen up by one line
            scroll_line(1);
            screen->cursor->t_posY--;
        }
    }

    // draw the character
    for (rows = 0; rows < FONT_ROWS; rows++) {
        pixel_ptr = screen->frame_buffer + screen->resX * ((screen->cursor->t_posY * FONT_ROWS) + rows) +
                    (screen->cursor->t_posX * FONT_COLS);
        bitmask = 0b10000000;
        for (cols = 0; cols < FONT_COLS; cols++) {
            if (*line_ptr & bitmask) {
                *pixel_ptr = d_front;
            } else {
                *pixel_ptr = d_back;
            }
            pixel_ptr++;
            bitmask >>= 1;
        }
        line_ptr++;
    }

    // update the cursor position
    screen->cursor->t_posX++;

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
	int pad_flag=0;
	int rest_count=0;
	int i=0;
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
					for (rest_count=0;rest_count<(8-strlen(buffer));rest_count++) {
						putchar('0');
					}
                    putstr(buffer, strlen(buffer));
                    break;
                case 'c':
                    int_arg = va_arg(arg_list, int); // use int_arg instead of char_arg
                    char_arg = (char)int_arg; // convert int_arg back to char
                    putchar(char_arg);
                    break;
                case 'u':
                    uint_arg = va_arg(arg_list, unsigned int);
                    utoa(uint_arg, buffer, 10);
					for (rest_count=0;rest_count<(8-strlen(buffer));rest_count++) {
						putchar('0');
					}
                    putstr(buffer, strlen(buffer));
                    break;
                case 'x':
                    uint_arg = va_arg(arg_list, unsigned int);
                    utoa(uint_arg, buffer, 16);
					for (rest_count=0;rest_count<(8-strlen(buffer));rest_count++) {
						putchar('0');
					}
                    putstr(buffer, strlen(buffer));
                    break;
                case 'p':
					putchar('0');
					putchar('x');
                    ptr_arg = va_arg(arg_list, void *);
                    utoa((unsigned long)ptr_arg, buffer, 16);
					for (rest_count=0;rest_count<(16-strlen(buffer));rest_count++) {
						putchar('0');
					}
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
				//roll line
				scroll_line(1);
				for (i=0;i<(screen->cursor->text_resX);i++) {
					putchar(' ');
				}
				screen->cursor->t_posX=0;
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