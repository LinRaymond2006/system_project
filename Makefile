.PHONY:all, cleanup, X, X_debug, TERM, TERM_debug
NASMFLAGS := -f bin
DIST_BIN :=CODE.BIN
SOURCE := MAIN.ASM
LOADER_SOURCE := LOADER.ASM
LOADER_NAME := LOADER.SYS
#ata0-master: type=disk, path="boot.img", mode=flat
all:
	-rm boot.img boot.img.lock
	-mkdir mount_dir
	cp boot.img.bck boot.img
	mkfs.fat -F32 -S512 -D0x80 -s4 boot.img

	nasm $(NASMFLAGS) $(SOURCE) -o $(DIST_BIN)
	#MAKE LOADER.SYS
	#nasm $(NASMFLAGS) $(LOADER_SOURCE) -o $(LOADER_NAME)

	#for debugging
	#nasm -f bin KERNEL_TMP.ASM -o KERNEL.SYS

	dd if=$(DIST_BIN) of=boot.img bs=1 seek=90 skip=90 count=420 conv=notrunc
	sudo mount -t vfat ./boot.img ./mount_dir/ -o rw,uid=$(shell id -u),gid=$(shell id -g)
	cp $(LOADER_NAME) ./mount_dir/

	#for debugging
	cp KERNEL.SYS ./mount_dir/


	sync
	sudo umount ./mount_dir/
cleanup:
	-rm $(DIST_BIN) $(LOADER_NAME) bochs.log debug.log boot.img dump.mem
	-sudo umount ./mount_dir/
X_debug:
	make all
	clear
	-rm boot.img.lock bochs.log debug.log
	bochs
X:
	make all
	clear
	-rm boot.img.lock bochs.log debug.log
	echo c | bochs
read:
	hexedit mem.dump
TERM_debug:
	make all
	-rm boot.img.lock bochs.log debug.log
	clear
	bochs -qf bochsrc.term

MEM_dump:
	make all
	-rm boot.img.lock bochs.log debug.log
	clear
	-echo 'c;writemem "dump.mem" 0 0xffffff' | bochs -qf bochsrc.term
TERM:
	make all
	-rm boot.img.lock bochs.log debug.log
	clear
	echo c | bochs -qf bochsrc.term
