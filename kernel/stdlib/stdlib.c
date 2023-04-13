typedef unsigned long size_t;

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

void *memmove(void *dest, void *src, size_t n) {
    //__asm__ __volatile__();

}


