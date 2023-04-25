CC := gcc
CFLAGS := -mcmodel=large -ffreestanding -fno-builtin -m64 -nostdlib -nodefaultlibs -I ./stdlib/ 
# -shared -c
.PHONY:all

all: ../../../build/tmp/kernel/int.o

../../../build/tmp/kernel/int.o: int.c fault_irq.h fault.h gate.h
	$(CC) $(CFLAGS) -c -shared $< -o $@