CC := gcc
CFLAGS := -mcmodel=large -ffreestanding -fno-builtin -m64 -nostdlib -nodefaultlibs -I ../../stdlib/ 
CFLAGS_NO_OPTIMIZE := -O0

# -shared -c
.PHONY:all

all: ../../../build/tmp/kernel/int.o


../../../build/tmp/kernel/int.o: int.c fault_irq.h fault.h gate.h
	#expand the fault.s first.
	$(CC) $(CFLAGS) $(CFLAGS_NO_OPTIMIZE) -c -shared $< -o $@