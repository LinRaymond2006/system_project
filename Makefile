.PHONY:all, run, cleanup
mnt_point := mount_dir
rootfs_dir := build/rootfs
build_dir := build
log_dir := run/log
dsk_image := hd.iso
LOADER_NAME := LOADER.SYS
KERNEL_NAME := KERNEL.SYS
all: FORCE
	make cleanup
	make all -C boot
	make all -C kernel

	sudo mount -t vfat ./build/hd.iso $(mnt_point) -o rw,uid=$(shell id -u),gid=$(shell id -g)

	cp $(rootfs_dir)/* $(mnt_point)/

	sync
	sudo umount $(mnt_point)

test:
	make all
	make run
run: FORCE
	-bochs -f ./run/bochsrc
run_t: FORCE
#	terminal emulation if you don't use a desktop interface!
	bochs -f ./run/bochsrc.term
cleanup: FORCE
	-sudo umount $(mnt_point)/
	-rm -rf $(build_dir)/*
	-rm -rf ./run/log/
	-mkdir ./run/log/
	-mkdir $(build_dir)
	-mkdir $(build_dir)/tmp
	-mkdir $(build_dir)/tmp/boot
	-mkdir $(build_dir)/tmp/kernel
	-mkdir $(build_dir)/rootfs
	-mkdir $(mnt_point)

sym: FORCE
	nm build/tmp/kernel/kernel.elf

symbol ?= Start_Kernel
file ?= $(build_dir)/tmp/kernel/kernel.elf

disasm:	FORCE
	objdump -Mintel --disassemble=$(symbol) $(file)

addr ?= 0

findsym: FORCE
	nm $(file) | grep $(addr)

disasm_all: FORCE
	objdump -Mintel -D build/tmp/kernel/kernel.elf

FORCE: ; 
