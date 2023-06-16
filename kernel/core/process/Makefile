.PHONY: all

CC := gcc
CFLAGS := -mcmodel=large -ffreestanding -fno-builtin -m64 -nostdlib -nodefaultlibs -I ../../stdlib/ -I ../../def
CFLAGS_NO_OPTIMIZE := -O0
AS := as
ASFLAGS64 := --64
LDFLAGS := -static -b elf64-x86-64

all: ../../../build/tmp/kernel/pm.o

../../../build/tmp/kernel/pm.o: ../../../build/tmp/kernel/context.o
	$(LD) $(LDFLAGS) -r $^ -o $@

../../../build/tmp/kernel/context.o: context_switch.c asm_interface.S pcb.h
	$(CC) -c $(CFLAGS) $< -o ../../../build/tmp/kernel/ctxt_switch.o
	$(CC) -E asm_interface.S > ../../../build/tmp/kernel/ctxt_asm.s
	$(AS) $(ASFLAGS64) ../../../build/tmp/kernel/ctxt_asm.s -o ../../../build/tmp/kernel/ctxt_asm.o
	$(LD) -r $(LDFLAGS) ../../../build/tmp/kernel/ctxt_switch.o ../../../build/tmp/kernel/ctxt_asm.o -o $@


