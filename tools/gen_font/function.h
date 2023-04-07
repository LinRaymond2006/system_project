#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <string.h>
#include <math.h>

#define ROWS 16
#define COLS 8


#define FONT_NAME "ascii_font"
#define OUTPUT_FILE "font.h"
//rows and cols are not correct

char cells[ROWS][COLS] = {0};

typedef struct cs_struct {
    int row;
    int col;
} CS_STRUCT;

CS_STRUCT cs;

#define DISP_CHR ' '
void draw_screen(char frame_char) {
    int i, j;
    for(i = 0; i < ROWS; i++) {
        for(j = 0; j < COLS; j++) {
            if (cells[i][j]==(char)'0') {
                attroff(COLOR_PAIR(1));
                mvaddch(i, j*2, ' ');
                mvaddch(i, (j*2)+1, ' ');
            } else if (cells[i][j]==(char)'1') {
                attroff(COLOR_PAIR(1));
                mvaddch(i, j*2, 'X');
                mvaddch(i, (j*2)+1, 'X');
            }
        }
    }
    for(i = 0; i < COLS; i++) {
        mvaddch(ROWS, i*2, frame_char);
        mvaddch(ROWS, (i*2)+1, frame_char);
    }
    i=0;
    for(i = 0; i < ROWS+1; i++) {
        mvaddch(i, (COLS*2), frame_char);
    }
	refresh_cursor();
	refresh();
    return;
}


void clear_cells() {
    memset(&cells, (char)'0', ROWS*COLS);
	refresh_cursor();
    return;
}
void refresh_cursor() {
    int i,j;
    for(int i = 0; i < ROWS; i++) {
        for(int j = 0; j < COLS; j++) {
            if (cells[i][j]==(char)'0') {
                attroff(COLOR_PAIR(1));
                mvaddch(i, j*2, ' ');
                mvaddch(i, (j*2)+1, ' ');
            } else if (cells[i][j]==(char)'1') {
                attroff(COLOR_PAIR(1));
                mvaddch(i, j*2, 'X');
                mvaddch(i, (j*2)+1, 'X');
            }
        }
    }
    move(cs.row, cs.col*2);
	if (cells[cs.row][cs.col]=='0') {
		attron(COLOR_PAIR(1));
		mvaddch(cs.row, (cs.col)*2, (char)' ');
		mvaddch(cs.row, ((cs.col)*2)+1, (char)' ');
		attroff(COLOR_PAIR(1));
	} else if (cells[cs.row][cs.col]=='1') {
		attron(COLOR_PAIR(1));
		mvaddch(cs.row, (cs.col)*2, (char)'X');
		mvaddch(cs.row, ((cs.col)*2)+1, (char)'X');
		attroff(COLOR_PAIR(1));
	}
	refresh();
    return;
}
void setup() {
    memset(&cells, (char)'0', ROWS*COLS);
    initscr();
    cbreak();
    noecho();
    curs_set(0);
	keypad(stdscr, TRUE);

    start_color();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    attron(COLOR_PAIR(1));
    cs.row=0;
    cs.col=0;
    refresh_cursor();
    return;
}

unsigned char str2bin(char *pointer) {
    int val = 0;

    for (int i = 0; i < COLS; i++) {
        if (pointer[i] == '1') {
            val += pow(2, COLS - i - 1);
        }
    }    
    return (unsigned char)val;
}

void bin2str(int val, char *ptr) {
    int tmp=val;
    for (int i = 0;i < COLS;i++) {
        if (tmp%2==1) {
            ptr[COLS-i-1] = (char)'1';
            tmp--;
            tmp/=2;
        } else {
            ptr[COLS-i-1] = (char)'0';
            tmp/=2;
        }
    }
    return;
}

int ui(int ascii) {
    int ch;
	char finished=0;

    clear_cells();
    while (finished!=1) {
        draw_screen((char)ascii);
        ch=getch();
        switch (ch) {
            case KEY_LEFT:
                cs.col--;
                if (cs.col < 0) {
                    cs.col = 0;
                }
                refresh_cursor();
                break;
            case KEY_RIGHT:
                cs.col++;
                if (cs.col > COLS-1) {
                    cs.col = COLS-1;
                }
                refresh_cursor();
                break;
            case KEY_UP:
                cs.row--;
                if (cs.row < 0) {
                    cs.row = 0;
                }
                refresh_cursor();
                break;
            case KEY_DOWN:
                cs.row++;
                if (cs.row > ROWS-1) {
                    cs.row = ROWS-1;
                }
                refresh_cursor();
                break;
            case 'c':
                clear_cells();
                draw_screen((char)ascii);
                break;
            case KEY_BACKSPACE:
                cells[cs.row][cs.col] = '0';
                draw_screen((char)ascii);
                break;
            case 'g':
                save_data(ascii);
                finished=1;
                clear_cells();
                break;
            case 'f':
                save_data(ascii);
                return 0;
                break;
            default:
                if (cells[cs.row][cs.col] == '0') {
                    cells[cs.row][cs.col] = '1';
                } else if (cells[cs.row][cs.col] == '1'){
                    cells[cs.row][cs.col] = '0';
                }
                draw_screen((char)ascii);
        }
    }
    return 1;
}

void debug() {

    exit(0);
}

void save_data(int ascii) {
    //double char array "cells" is a global variable, therefore not passed again from caller
    for (int i=0;i<ROWS;i++) {
        data[(ascii*ROWS)+i] = str2bin(&cells[i][0]);
    }
    return;
}

void load_data(int ascii) {
    //ascii as an index, convert and load the binary to cells
    return;
}

void quit() {
    write_file();
    refresh();
	endwin();
}

void char2hexa(unsigned char val, char* output) {
  sprintf(output, "0x%02x", val);
  return;
}


FILE *fp;



#define LIST_LENGTH ROWS
//STR_LEN is fixed (0x??) + NULL(string terminator)
#define STR_LEN 5

void join(char **list, const char *separator, char *result) {
    // build the output string
    int offset = 0;
    for (int i = 0; i < LIST_LENGTH; i++) {
        memcpy(result + offset, &list[i], STR_LEN - 1);
        offset += STR_LEN - 1;
        if (i < LIST_LENGTH - 1) {
            strcpy(result + offset, separator);
            offset += strlen(separator);
        }
    }

    // add null terminator to the end of the string
    result[offset] = '\0';
    return;
}

//#define ROWS 3
//#define ROWS 16
#define DATA_SIZE 256 * ROWS

void write_file() {
    //unsigned char data[DATA_SIZE] = { /* ... */ };
    char line[256]; // Maximum length of a line in the file

    FILE *fp = fopen(OUTPUT_FILE, "w");
	if (fp == NULL) {
		quit();
		printf("error opening file: ");
		printf(OUTPUT_FILE);
		printf("\n");
		exit(-1);
	}
    fprintf(fp, "unsigned char %s[256][%d] = \n{\n", FONT_NAME, ROWS);
    for (int i = 0; i < 256; i++) {
        unsigned char *p = &data[i * ROWS];
        char *q = line;
        for (int j = 0; j < ROWS; j++) {
            q += sprintf(q, "0x%02x", p[j]);
            if (j != ROWS - 1) {
                *q++ = ',';
                *q++ = ' ';
            }
        }
        *q = '\0';
        if (fprintf(fp, "\t{%s}, \t\t// ascii order: \'0x%02x\', character: %c\n", line, i, i) < 0) {
            perror("Error writing to file");
            exit(EXIT_FAILURE);
        }
    }
    fprintf(fp, "};");
    fclose(fp);
    return 0;

}


