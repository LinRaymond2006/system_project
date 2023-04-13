#ifndef __STDLIB_H__

typedef unsigned long size_t;

size_t strlen(register char *str) {
    register size_t i=0;
    for (;;i++) {
        if (str[i]==(char)0) {
            return i;
        }
    }
}

void *memcpy(void *dest, void *src, size_t n) {
    __asm__ __volatile__ (                           \
        "cld        \n\t"                            \
        "movq %[source], %%rsi       \n\t"           \
        "movq %[destination], %%rdi       \n\t"      \
        "movq %[count], %%rcx       \n\t"            \
        "rep                  \n\t"                    \
        "movsb              \n\t"                    \
        :                                            \
        : [destination]"m"(dest), [source]"m"(src), [count]"m"(n)   \
        : "rax", "rcx", "rsi", "rdi", "memory"                      \
    );
    return dest;
}

void *memset(void *dest, int character, size_t n) {
    __asm__ __volatile__ (    \
        "cld        \n\t"   \
        "movb %[value], %%al      \n\t" \
        "movq %[count], %%rcx   \n\t" \
        "movq %[buffer], %%rdi\n\t" \
        "rep    \n\t"   \
        "stosb  \n\t"   \
        :
        :[buffer]"m"(dest), [value]"m"(character), [count]"m"(n)
        : "al", "rcx", "rdi", "memory"
    );
    return dest;
}

void *memmove(void *dest, void *src, size_t n) {
    __asm__ __volatile__
}


#endif