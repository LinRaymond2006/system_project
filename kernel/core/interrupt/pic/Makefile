CC := gcc
CFLAGS := -mcmodel=large -ffreestanding -fno-builtin -m64 -nostdlib -nodefaultlibs -I ../../../stdlib/ -I ../../../def
CFLAGS_NO_OPTIMIZE := -O0
AS := as
ASFLAGS64 := --64
LDFLAGS := -static -b elf64-x86-64
# -shared -c
.PHONY:all

all: ../../../../build/tmp/kernel/pic.o


../../../../build/tmp/kernel/pic.o: 8259A.c 8259A_cw.h
	$(CC) $(CFLAGS) -c $< -o ../../../../build/tmp/kernel/pic.o