.PHONY: all

CC := gcc
CFLAGS := -mcmodel=large -ffreestanding -fno-builtin -m64 -nostdlib -nodefaultlibs -I ../../stdlib/ -I ../../def
CFLAGS_NO_OPTIMIZE := -O0
AS := as
ASFLAGS64 := --64
LDFLAGS := -static -b elf64-x86-64

all: ../../../build/tmp/kernel/mm.o

../../../build/tmp/kernel/mm.o: ../../../build/tmp/kernel/e820.o
	$(LD) $(LDFLAGS) -r $^ -o $@

../../../build/tmp/kernel/e820.o: e820.c e820.h
	$(CC) -c $(CFLAGS) $< -o $@

