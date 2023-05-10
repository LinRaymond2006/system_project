CC := gcc
CFLAGS := -mcmodel=large -ffreestanding -fno-builtin -m64 -nostdlib -nodefaultlibs -I ../../../stdlib/ -I ../../../def
CFLAGS_NO_OPTIMIZE := -O0
AS := as
ASFLAGS64 := --64
LDFLAGS := -static -b elf64-x86-64
# -shared -c
.PHONY:all

all: ../../../../build/tmp/kernel/pic.o

#NOT UPDATED 2023/05/10

../../../../build/tmp/kernel/pic.o: 8259A.c 8259A_cw.h ../../../../build/tmp/kernel/8259A_handler.o ../../../../build/tmp/kernel/pic_isr.o
	$(CC) $(CFLAGS) -c $< -o ../../../../build/tmp/kernel/pic.o
	$(LD) $(LDFLAGS) -r ../../../../build/tmp/kernel/pic.o ../../../../build/tmp/kernel/8259A_handler.o  ../../../../build/tmp/kernel/pic_isr.o -o ../../../../build/tmp/kernel/pic_tmp.o
	mv ../../../../build/tmp/kernel/pic_tmp.o ../../../../build/tmp/kernel/pic.o
../../../../build/tmp/kernel/8259A_handler.o: 8259A_handler.c 8259A_cw.h
	$(CC) $(CFLAGS) -c 8259A_handler.c -o ../../../../build/tmp/kernel/8259A_handler.o
../../../../build/tmp/kernel/pic_isr.o: 8259A_Isr.S 
	$(CC) $(CFLAGS) -c 8259A_Isr.S -o ../../../../build/tmp/kernel/pic_isr.o