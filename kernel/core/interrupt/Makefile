CC := gcc
CFLAGS := -mcmodel=large -ffreestanding -fno-builtin -m64 -nostdlib -nodefaultlibs -I ../../stdlib/ -I ../../def
CFLAGS_NO_OPTIMIZE := -O0
AS := as
ASFLAGS64 := --64
LDFLAGS := -static -b elf64-x86-64
# -shared -c
.PHONY:all

all: ../../../build/tmp/kernel/int.o


../../../build/tmp/kernel/int.o: int.c fault_irq.h fault.S gate.h
	#expand the fault.s first
	$(CC) -I ../../stdlib/ -I ../../def -E fault.S > ../../../build/tmp/kernel/exception.s
	$(AS) $(ASFLAGS64) -fpie ../../../build/tmp/kernel/exception.s -o ../../../build/tmp/kernel/exception.o
	$(CC) $(CFLAGS) $(CFLAGS_NO_OPTIMIZE) -c -shared $< -o ../../../build/tmp/kernel/int_gate.o
	$(LD) $(LDFLAGS) -shared ../../../build/tmp/kernel/int_gate.o ../../../build/tmp/kernel/exception.o -o $@