.PHONY: all
AS := as
CC := gcc
LD := ld
OBJCOPY := objcopy
CFLAGS := -mcmodel=large -ffreestanding -fno-builtin -m64 -nostdlib -nodefaultlibs -I ./stdlib/ -I ./screen
KERNEL_LDFLAGS :=  -static -b elf64-x86-64 -L ../build/tmp/kernel/
ASFLAGS := --64

all:  ../build/tmp/kernel/libos.a ../build/rootfs/KERNEL.SYS

../build/rootfs/KERNEL.SYS: ../build/tmp/kernel/head.o ../build/tmp/kernel/kernel.o #remember to write all the object file in specifc order ($^ = all the source file list)
#kernel.elf: complete image of kernel (in ELF64 file format)
#KERNEL.SYS: machine code extracted from "kernel.lef"
	ld $(KERNEL_LDFLAGS) -T kernel.lds -o ../build/tmp/kernel/kernel.elf $^ -los
	$(OBJCOPY) -I elf64-x86-64 -S -R ".eh_frame" -R ".comment" -O binary ../build/tmp/kernel/kernel.elf $@

../build/tmp/kernel/head.o: head.S
	-rm ../build/tmp/kernel/head.s
	$(CC) -E $^ > ../build/tmp/kernel/head.s
	$(AS) $(ASFLAGS) ../build/tmp/kernel/head.s -o $@

../build/tmp/kernel/kernel.o: main.c
	$(CC) -shared -c $(CFLAGS) $^ -o $@

../build/tmp/kernel/libos.a: ../build/tmp/kernel/string.o ../build/tmp/kernel/stdlib.o ../build/tmp/kernel/printf.o
	ar -r $@ $^

../build/tmp/kernel/printf.o: stdlib/src/printf.c
	$(CC) -c -static $(CFLAGS) $^ -o $@

../build/tmp/kernel/stdlib.o: stdlib/src/stdlib.c
	$(CC) -c -static $(CFLAGS) $^ -o $@

../build/tmp/kernel/string.o: stdlib/src/string.c
	$(CC) -c -static $(CFLAGS) $^ -o $@

