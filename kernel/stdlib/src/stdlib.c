#include "ctype.h"
typedef unsigned long size_t;
typedef long ssize_t;





//change the datatype of "value" and the function return value to specific datatype if needed, or simply just do some casting stuff

// caller should cast the return value if other datawidth is wanted
ssize_t atoi(const char* str, int radix) {
    int sign = 1;
    ssize_t value = 0;
    const char* p = str;

    while (isspace(*p)) {
        p++;
    }

    if (*p == '-') {
        sign = -1, p++;
    } else if (*p == '+') {
        p++;
    }

    while (1) {
        int digit;
        if (isdigit(*p)) {
            digit = *p - '0';
        } else if (*p >= 'a' && *p < 'a' + radix - 10) {
            digit = *p - 'a' + 10;
        } else if (*p >= 'A' && *p < 'A' + radix - 10) {
            digit = *p - 'A' + 10;
        } else {
            break;
        }
        if (digit >= radix) break;
        value = value * radix + digit;
        p++;
    }

    return sign * value;
}


// caller should cast the return value if other datawidth is wanted
size_t atou(const char* str, int radix) {
    size_t value = 0;
    const char* p = str;

    while (isspace(*p)) {
        p++;
    }

    while (1) {
        int digit;
        if (isdigit(*p)) {
            digit = *p - '0';
        } else if (*p >= 'a' && *p < 'a' + radix - 10) {
            digit = *p - 'a' + 10;
        } else if (*p >= 'A' && *p < 'A' + radix - 10) {
            digit = *p - 'A' + 10;
        } else {
            break;
        }
        if (digit >= radix) break;
        value = value * radix + digit;
        p++;
    }

    return value;
}

char *itoa(ssize_t value, char *str, int radix) {
    char *ptr = str, *ptr1 = str, tmp_char;
    ssize_t tmp_value;

    if (value < 0 && radix == 10) {
        *ptr++ = '-';
        value = -value;
    }

    do {
        tmp_value = value;
        value /= radix;
        *ptr++ = "0123456789abcdef"[tmp_value - value * radix];
    } while (value);

    *ptr-- = '\0';

    while (ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr-- = *ptr1;
        *ptr1++ = tmp_char;
    }

    return str;
}

char *utoa(size_t value, char *str, int radix) {
    char *ptr = str, *ptr1 = str, tmp_char;
    size_t tmp_value;

    do {
        tmp_value = value;
        value /= radix;
        *ptr++ = "0123456789abcdef"[tmp_value - value * radix];
    } while (value);

    *ptr-- = '\0';

    while (ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr-- = *ptr1;
        *ptr1++ = tmp_char;
    }

    return str;
}
