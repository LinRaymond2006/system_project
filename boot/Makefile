.PHONY:all
NASMFLAGS := -f bin
MBR_CODE := CODE.BIN
MBR_SOURCE := MAIN.ASM
LOADER_SOURCE := LOADER.ASM
LOADER_NAME := LOADER.SYS
mnt_point := ../mnt
obj_dir := ../build/obj
build_dir := ../build
dsk_image := hd.iso
#ata0-master: type=disk, path="boot.img", mode=flat
all:
	-rm $(dsk_image) $(dsk_image).lock
	-mkdir $(obj_dir)
	cp $(dsk_image).bck $(dsk_image)
	mkfs.fat -F32 -S512 -D0x80 -s4 $(dsk_image)

	dd if=$(MBR_CODE) of=$(dsk_image) bs=1 seek=90 skip=90 count=420 conv=notrunc
	
	cp $(LOADER_NAME) $(obj_dir)
	cp $(dsk_image) $(build_dir)/


$(LOADER_NAME): $(LOADER_SOURCE)
	nasm $(NASMFLAGS) $(LOADER_SOURCE) -o $(LOADER_NAME)

$(MBR_CODE): $(MBR_SOURCE)
	nasm $(NASMFLAGS) $(MBR_SOURCE) -o $(MBR_CODE)
