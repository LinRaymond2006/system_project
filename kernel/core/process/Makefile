.PHONY: all

CC := gcc
CFLAGS := -mcmodel=large -ffreestanding -fno-builtin -m64 -nostdlib -nodefaultlibs -I ../../stdlib/ -I ../../def
CFLAGS_NO_OPTIMIZE := -O0
AS := as
ASFLAGS64 := --64
LDFLAGS := -static -b elf64-x86-64

all: ../../../build/tmp/kernel/pm.o

../../../build/tmp/kernel/pm.o: ../../../build/tmp/kernel/ctxt_asm.o ../../../build/tmp/kernel/ctxt_switch.o
	$(LD) -r $(LDFLAGS) $^ -o $@

../../../build/tmp/kernel/ctxt_switch.o: context_switch.c
	$(CC) -c $(CFLAGS) $< -o $@

../../../build/tmp/kernel/ctxt_asm.o: asm_interface.S
	$(CC) -c $(CFLAGS) $< -o $@
	#$(CC) -E $< > ../../../build/tmp/kernel/ctxt_asm.s
	#$(AS) $(ASFLAGS64) ../../../build/tmp/kernel/ctxt_asm.s -o $@
