#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define ROWS 16
#define COLS 8


#define FONT_NAME "VGABIOS_ascii"
#define OUTPUT_FILE "font.h"


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
        if (fprintf(fp, "\t{%s}, \t\t// ascii order: \'0x%02x\'\n", line, i, i) < 0) {
            perror("Error writing to file");
            exit(EXIT_FAILURE);
        }
    }
    fprintf(fp, "};");
    fclose(fp);
    return 0;

}


