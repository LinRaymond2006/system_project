.PHONY:all, run, cleanup
mnt_point := mnt
rootfs_dir := build/rootfs
build_dir := build
log_dir := run/log
dsk_image := hd.iso
LOADER_NAME := LOADER.SYS
KERNEL_NAME := KERNEL.SYS
all:
	make all -C boot
	make all -C kernel
	sudo mount -t vfat $(build_dir)/$(dsk_image) $(mnt_point) -o rw,uid=$(shell id -u),gid=$(shell id -g)
	
	cp $(rootfs_dir)/* $(mnt_point)/

	sync
	sudo umount $(mnt_point)

run:
	make all
	bochs -f ./run/bochsrc
#	terminal emulation if you don't use a desktop interface!
#	bochs -f ./run/bochsrc.term
cleanup:
	-sudo rm -rf $(build_dir)/*
	-mkdir $(build_dir)/tmp
	-mkdir $(build_dir)/tmp/kernel
	-mkdir $(build_dir)/rootfs
	-sudo rm $(build_dir)/*