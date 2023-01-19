.PHONY:all, run, cleanup
mnt_point := mnt
obj_dir := build/obj
build_dir := build
dsk_image := hd.iso
LOADER_NAME := LOADER.SYS
KERNEL_NAME := KERNEL.SYS
all:
	make all -C boot
	
	sudo mount -t vfat $(build_dir)/$(dsk_image) $(mnt_point) -o rw,uid=$(shell id -u),gid=$(shell id -g)
	
	cp $(obj_dir)/* $(mnt_point)/

	sync
	sudo umount $(mnt_point)

run:
	make all
	bochs -f ./run/bochsrc
#	bochs -f ./run/bochsrc.term
cleanup:
	-sudo rm build/*
	-sudo rm run/log/*