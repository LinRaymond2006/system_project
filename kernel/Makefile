.PHONY: all
AS := as
CC := gcc
LD := ld
OBJCOPY := objcopy
CFLAGS := -mcmodel=large -ffreestanding -fno-builtin -m64 -c -nostdlib -nodefaultlibs -I ./stdlib/
KERNEL_LDFLAGS :=  -b elf64-x86-64
ASFLAGS := --64

all: ../build/rootfs/KERNEL.SYS

../build/rootfs/KERNEL.SYS: ../build/tmp/kernel/head.o ../build/tmp/kernel/kernel.o ../build/tmp/kernel/printf.o#remember to write all the object file in specifc order ($^ = all the source file list)
#kernel.elf: complete image of kernel (in ELF64 file format)
#KERNEL.SYS: machine code extracted from "kernel.lef"
	$(LD) $(KERNEL_LDFLAGS) -T kernel.lds -o ../build/tmp/kernel/kernel.elf $^
	$(OBJCOPY) -I elf64-x86-64 -S -R ".eh_frame" -R ".comment" -O binary ../build/tmp/kernel/kernel.elf $@

../build/tmp/kernel/head.o: head.S
	-rm ../build/tmp/kernel/head.s
	$(CC) -E $^ > ../build/tmp/kernel/head.s
	$(AS) $(ASFLAGS) ../build/tmp/kernel/head.s -o $@

../build/tmp/kernel/kernel.o: main.c
	$(CC) -shared $(CFLAGS) $^ -o $@

../build/tmp/kernel/printf.o: screen/printf.c
	$(CC) -shared $(CFLAGS) $^ -o $@