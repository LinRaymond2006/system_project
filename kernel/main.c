#include "screen/fonts/colors.h"
#include "printf.h"
#include "stdlib.h"
#include "string.h"
void Start_Kernel(void) {
    char *aaa="aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaab";
    //putstr(aaa, strlen(aaa));
    //putstr(aaa, strlen(aaa));
    printf("%c %c %x %p %d %s", 'a', 'A', 0xff,aaa, 123,aaa);
    while (1) {
        ;
    }
}