#include "printf.h"
#include <stddef.h>
typedef unsigned long size_t;
typedef long ssize_t;
typedef size_t reg_sz64;
#define ERROR_STACK_IST1 reg_sz64 tss_rsp, reg_sz64 errcode

extern void do_divide_error(reg_sz64 rsp,reg_sz64 errcode)
{
	unsigned long *rip = (unsigned long *)(rsp + 0x98);
    printf("%s sourced from %p, with error code %p and stack pointer %p\n", __PRETTY_FUNCTION__, *rip, errcode, rsp);                  \
	while(1);
}
