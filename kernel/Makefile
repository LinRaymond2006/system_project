.PHONY: all
rootfs_dir := ../build/rootfs
tmp_obj_dir := ../build/tmp/kernel
kernel_file := KERNEL.SYS
kernel_ld_script := kernel.lds
head_obj := head.o
head_source := head.S
head_expanded := head.s
kernel_main_obj := kernel.o
kernel_main_source := main.c
kernel_tmp_image := kernel.elf
AS := as
CC := gcc
LD := ld
OBJCOPY := objcopy
CFLAGS := -mcmodel=large -fno-builtin -m64
KERNEL_LDFLAGS :=  -b elf64-x86-64 -T kernel.lds
ASFLAGS := --64
all: $(kernel_file)
	cp $(tmp_obj_dir)/$(kernel_file) $(rootfs_dir)/

$(kernel_file): $(head_obj) $(kernel_main_obj)
#kernel_tmp_image: an elf file
#kernel_file: machine code extracted from "kernel_tmp_image"
	$(LD) $(KERNEL_LDFLAGS) -o $(tmp_obj_dir)/$(kernel_tmp_image) $(tmp_obj_dir)/$(head_obj) $(tmp_obj_dir)/$(kernel_main_obj)
	$(OBJCOPY) -I elf64-x86-64 -S -R ".eh_frame" -R ".comment" -O binary $(tmp_obj_dir)/$(kernel_tmp_image) $(tmp_obj_dir)/$(kernel_file)

$(head_obj): $(head_source)
	-rm $(tmp_obj_dir)/$(head_expanded)
	$(CC) -E $(head_source) > $(tmp_obj_dir)/$(head_expanded)
	$(AS) $(ASFLAGS) $(tmp_obj_dir)/$(head_expanded) -o $(tmp_obj_dir)/$(head_obj)

$(kernel_main_obj): $(kernel_main_source)
	$(CC) $(CFLAGS) $(kernel_main_source) -o $(tmp_obj_dir)/$(kernel_main_obj)