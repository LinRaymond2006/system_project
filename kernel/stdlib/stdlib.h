#ifndef __STDLIB_H__


//some problem with strlen
static unsigned long strlen(register char *str) {
    register int i=0;
    for (;;i++) {
        if (*str+i==(char)0) {
            return i;
        }
    }
}


#endif