/* stdarg from the standard library */
#include <stdarg.h>
/* stdint from the standard library */
#include <stdint.h>

//with the pointer design, each part of the system can have its own cursor screen descriptor, making the printing more flexable.
typedef struct {
    unsigned int resX;
    unsigned int resY;
    unsigned int pixel_bytes; //bytes per pixel
    
    void *buffer_start;
    void *vbuffer_start;        //virtual address, optional
    unsigned long long buffer_length;

} SCR_INFO;

SCR_INFO *scr_info=&scr, scr;


typedef struct {
    //note: the posX and posY are the cursor that points to the "text buffer", which is an abstraction of the text on the screen
    unsigned int posX;
    unsigned int posY;

    unsigned int back_color;
    unsigned int front_color;
    char *page_buffer;  //text buffer
} CURSOR;

CURSOR *cursor=&crs, crs;

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
//if the function "abs_addr" is called frequently, the printing stuff won't be so efficent, so a better solution must be used
//solution: cache the value of the upper left corner, so few math operations are needed.
//for printing a new row in a character: add d_screen->resX to the cached value
//for printing next bit in the same row, don't write back to the cached index, or a pollution problem will happen, instead, use (cached_value+index_in_for_loop) for convenience
void *abs_addr(int x, int y, SCR_INFO *d_screen) {
    //calcutated the linear address of the font buffer, 
    #define RELATED_X (x*VBEFONT_X)   /*related X position */
    #define RELATED_Y (y*VBEFONT_Y)   /*related Y position */
    return d_screen->buffer_start +((RELATED_Y*(d_screen->resX)+RELATED_X)*(d_screen->pixel_bytes));
    
}

void *abs_vaddr(int x, int y, SCR_INFO *d_screen) {
    //calcutated the linear address of the font buffer, 
    #define RELATED_X (x*VBEFONT_X)   /*related X position */
    #define RELATED_Y (y*VBEFONT_Y)   /*related Y position */
    return d_screen->vbuffer_start +((RELATED_Y*(d_screen->resX)+RELATED_X)*(d_screen->pixel_bytes));
    
}

int putchar(int character, int back_color, int front_color) {
    unsigned char ascii=(unsigned char)character;

    int x, y, bits;
    /*
        -the VBE font is in 8(cols)*16(rows) format, which means the 
         X value and the Y value are hardwritten into the program.
        -each 'X' mark represents a bit, comprised of 16 unsigned char

        OXXXXXXX
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
        
        the putchar is responsible for caching the value of the upper left corner of the font, which is "O" in the font above
    */
    //linear memory address works simpler for now , use the virtual one once the program is more stable 
    //void *abs_pos=abs_vaddr(cursor->posX, cursor->posY, scr_info);
    void *abs_pos=abs_addr(cursor->posX, cursor->posY, scr_info);
    //add case condition for each pixel width (1/2/3(4)/4) bytes, only pixel width listed ahead are accepted
    for (y=0;y<VBEFONT_Y;y++) {
        for (x=0;x<VBEFONT_X;x++) {
            if ((ascii & (0b1<<x)) == 1) {
                //the data type may change as the pixel width are not the same
                //(char *) for 1, (short *) for 2, (int *) for 3 and 4
                *(int *)(abs_pos+VBEFONT_X-x)=(int)front_color;
            } else {
                *(int *)(abs_pos+VBEFONT_X-x)=(int)back_color;
            }
        }
        abs_pos+=(scr_info->resX)*(scr_info->pixel_bytes);
    }
    //do some cursor increment and wrapping stuff here

}

//