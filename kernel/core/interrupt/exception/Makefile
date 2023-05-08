CC := gcc
CFLAGS := -mcmodel=large -ffreestanding -fno-builtin -m64 -nostdlib -nodefaultlibs -I ../../../stdlib/ -I ../../../def
CFLAGS_NO_OPTIMIZE := -O0
AS := as
ASFLAGS64 := --64
LDFLAGS := -static -b elf64-x86-64
# -shared -c
.PHONY:all

all: ../../../../build/tmp/kernel/int.o


../../../../build/tmp/kernel/int.o: handler.c fault_irq.h isr_launcher.S reg_irq.h error_handler.h
	$(CC) $(CFLAGS) -c isr_launcher.S -o ../../../../build/tmp/kernel/isr.o
	$(CC) $(CFLAGS) -c -shared $< -o ../../../../build/tmp/kernel/err_handler.o
	$(LD) $(LDFLAGS) -r ../../../../build/tmp/kernel/err_handler.o ../../../../build/tmp/kernel/isr.o -o $@