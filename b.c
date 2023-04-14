typedef unsigned long size_t;
typedef long ssize_t
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

//caller should cast the return value if integer is wanted
size_t atoi(const char *str, int radix) {
    int sign = 1;
    size_t value = 0;
    const char *p = str;

    while (isspace(*p)) p++;

    if (*p == '-') {
        sign = -1, p++;
    } else if (*p == '+') {
        p++;
    }

    while (isdigit(*p) ||   \
    (*p >= 'a' && *p < 'a' + radix - 10) || \
    (*p >= 'A' && *p < 'A' + radix - 10)) {
        int digit = isdigit(*p) ? *p - '0' : tolower(*p) - 'a' + 10;
        if (digit >= radix) break;
        value = value * radix + digit;
        p++;
    }

    return (size_t) (sign * (int)value);
}

//caller should cast the return value if integer is wanted
size_t atou(const char *str, int radix) {
    size_t value = 0;
    const char *p = str;

    while (isspace(*p)) p++;

    while (isdigit(*p) || \
    (*p >= 'a' && *p < 'a' + radix - 10) || \
    (*p >= 'A' && *p < 'A' + radix - 10)) {
        int digit = isdigit(*p) ? *p - '0' : tolower(*p) - 'a' + 10;
        if (digit >= radix) break;
        value = value * radix + digit;
        p++;
    }

    return value;
}


int main() {
	printf("%d", (int)atou("10", 8));
}
