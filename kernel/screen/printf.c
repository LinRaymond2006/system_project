/* stdarg from the standard library */
#include <stdarg.h>
/* stdint from the standard library */
#include <stdint.h>


typedef struct {
    unsinged int resX;
    unsigned int resY;
    unsigned int pixel_bytes; //bytes per pixel
    
    void *buffer_start;
    void *vbuffer_start;        //virtual address, optional
    unsigned long long buffer_length;

} SCR_INFO;

SCR_INFO *scr_info;


typedef struct {
    //note: the posX and posY are the cursor that points to the "text buffer", which is an abstraction of the text on the screen
    unsigned int posX;
    unsigned int posY;

    unsigned int back_color;
    unsigned int front_color;
    char *page_buffer;  //text buffer
} CURSOR;

CURSOR *cursor;

//VBE mode 0x180 as defualt display mode
#define NUM_X 1440
#define NUM_Y 900
#define PIXEL_WIDTH 32

#define TEXT_X 180
//56.25 actually, but take the 
#define TEXT_Y 56

#define VBEFONT_X 8
#define VBEFONT_Y 16

char *defualt_tbuff_ptr=&default_buffer;
char default_buffer[TEXT_Y][TEXT_X];

int move(int y, int x) {
    cursor->posX=x;
    cursor->posY=y;
    //do the wipe and write stuff here
    return 0;
}

//note: x, y are the text position, not pixel position!
//for convenience of switching screen descriptor in future, the function are not written in macro format
int abs_addr(int x, int y, SCR_INFO *d_screen) {
    //calcutated the linear address of the font buffer, 
    int ret_val=0;
    int (x*VBEFONT_X*(d_screen->pixel_bytes))
    (y*VBEFONT_Y)
}

int putchar(int character) {
    unsigned char ascii=(unsigned char)character;

    int x, y, bits;
    /*
        -the VBE font is in 8(cols)*16(rows) format, which means the 
         X value and the Y value are hardwritten into the program.
        -each 'X' mark represents a bit, comprised of 16 unsigned char

        XXXXXXXX
        XXXXXXXX
        XXXXXXXX
        XXXXXXXX
        XXXXXXXX
        XXXXXXXX
        XXXXXXXX
        XXXXXXXX
        XXXXXXXX
        XXXXXXXX
        XXXXXXXX
        XXXXXXXX
        XXXXXXXX
        XXXXXXXX
        XXXXXXXX
        XXXXXXXX
    */
    for (y=0;y<16;y++) {
        for (x=0;x<16;x++) {
            //do some bitwise stuff here
            //8 bits are small enough for bit enumration
            if ((ascii & 0b00000001) == 1) cursor->
        }
    }
    //do some cursor increment and wrapping stuff here

}

//