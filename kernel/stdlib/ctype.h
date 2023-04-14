#ifndef __CTYPE_H__
#define __CTYPE_H__

#define isalnum(c)  (isalpha(c) || isdigit(c))
#define isalpha(c)  (islower(c) || isupper(c))
#define iscntrl(c)  ((c) < ' ' || (c) == 0x7f)
#define isdigit(c)  ((c) >= '0' && (c) <= '9')
#define isgraph(c)  (isalnum(c) || ispunct(c))
#define islower(c)  ((c) >= 'a' && (c) <= 'z')
#define isprint(c)  ((c) >= ' ' && (c) <= '~')
#define ispunct(c)  (isprint(c) && !isalnum(c) && !isspace(c))
#define isspace(c)  ((c) == ' ' || (c) == '\t' || (c) == '\n' || (c) == '\r' || (c) == '\v' || (c) == '\f')
#define isupper(c)  ((c) >= 'A' && (c) <= 'Z')
#define isxdigit(c) (isdigit(c) || ((c) >= 'a' && (c) <= 'f') || ((c) >= 'A' && (c) <= 'F'))
#define tolower(c)  ((c) >= 'A' && (c) <= 'Z' ? (c) + ('a' - 'A') : (c))
#define toupper(c)  ((c) >= 'a' && (c) <= 'z' ? (c) - ('a' - 'A') : (c))

#endif