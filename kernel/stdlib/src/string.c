#include <ctype.h>

typedef unsigned long size_t;
typedef long ssize_t;

size_t strlen(const char* str) {
    size_t len=0;
    __asm__ volatile (
        "repne scasb\n\t"   // Find the end of the string
        "not %%rcx\n\t"        // Convert len from -1 to the actual length
	    "dec %%rcx\n\t"
        "movq %%rcx, %[ret_val]"
        : [ret_val]"=r"(len)
        : "D"(str), "c"(0xffffffffffffffff), "a"(0)
        : "memory", "cc"
    );
    return len;
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
        : "rax", "rcx", "rsi", "rdi", "memory", "cc"                   \
    );
    return dest;
}

void *memcpy_int(void *dest, void *src, size_t n) {
    __asm__ __volatile__ (                           \
        "cld        \n\t"                            \
        "movq %[source], %%rsi       \n\t"           \
        "movq %[destination], %%rdi       \n\t"      \
        "movq %[count], %%rcx       \n\t"            \
        "rep                  \n\t"                    \
        "movsl              \n\t"                    \
        :                                            \
        : [destination]"m"(dest), [source]"m"(src), [count]"m"(n)   \
        : "rax", "rcx", "rsi", "rdi", "memory", "cc"                   \
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
        : "al", "rcx", "rdi", "memory", "cc"
    );
    return dest;
}

void *memset_int(void *dest, int character, size_t n) {
    __asm__ __volatile__ (    \
        "cld        \n\t"   \
        "movb %[value], %%al      \n\t" \
        "movq %[count], %%rcx   \n\t" \
        "movq %[buffer], %%rdi\n\t" \
        "rep    \n\t"   \
        "stosl  \n\t"   \
        :
        :[buffer]"m"(dest), [value]"m"(character), [count]"m"(n)
        : "al", "rcx", "rdi", "memory", "cc"
    );
    return dest;
}

void *memmove(void* dest, const void* src, size_t n) {
    if (dest == src) {  //bruh
        return dest;
    } else if (dest < src) {    //copy from start of buffer
        __asm__ __volatile__ (      \
            "rep        \n\t"       \
            "movsb      \n\t"       
            : 
            : "S"(src), "D"(dest), "c"(n)
            : "memory", "cc"
        );
    } else {    //copy from the end of buffer
        __asm__ __volatile__ (          \
            "std         \n\t"          \
            "rep        \n\t"           \
            "movsb      \n\t"           \
            "cld        \n\t"           \
            : 
            : "S"((const char*)src + n - 1), "D"((char*)dest + n - 1), "c"(n)
            : "memory", "cc"
        );
    }
    return dest;
}

void *memmove_int(void* dest, const void* src, size_t n) {
    if (dest == src) {  //bruh
        return dest;
    } else if (dest < src) {    //copy from start of buffer
        __asm__ __volatile__ (      \
            "rep        \n\t"       \
            "movsl      \n\t"       
            : 
            : "S"(src), "D"(dest), "c"(n)
            : "memory", "cc"
        );
    } else {    //copy from the end of buffer
        __asm__ __volatile__ (          \
            "std         \n\t"          \
            "rep        \n\t"           \
            "movsl      \n\t"           \
            "cld        \n\t"           \
            : 
            : "S"((const char*)src + n - 1), "D"((char*)dest + n - 1), "c"(n)
            : "memory", "cc"
        );
    }
    return dest;
}
