#ifndef __PRINTF_H__
#define __PRINTF_H__

#include "fonts/colors.h"

struct ColorSet {
	int OldFront;
	int OldBack;
};

extern struct ColorSet BackupColorSet;

typedef struct {
	unsigned int text_resX;
	unsigned int text_resY;
	unsigned char *buffer;
    unsigned int t_posX;
    unsigned int t_posY;
	unsigned int posX;
	unsigned int posY;
	unsigned int default_front;
	unsigned int default_back;
} CURSOR;

typedef struct {
	int resX;
	int resY;
	int vbe_mode;
	unsigned int *frame_buffer;		//both virtual and physical address are accepted as long as it is in x86_64 canoncial address format
	CURSOR *cursor;
} DISPALY_DESCRIPTOR;



extern DISPALY_DESCRIPTOR *screen;

extern void scroll_line();

extern void putchar();
extern void putstr();
extern void printf();
extern void change_color();


#endif