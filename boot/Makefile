.PHONY:all
NASMFLAGS := -f bin
NAS := nasm
#ata0-master: type=disk, path="boot.img", mode=flat
all:../build/hd.iso


../build/hd.iso:../build/tmp/boot/CODE.BIN ../build/rootfs/LOADER.SYS
	-rm $@ ../build/hd.iso.lock
	-mkdir ../build/rootfs
	cp hd.iso.bck $@
	mkfs.fat -F32 -S512 -D0x80 -s4 $@

	dd if=../build/tmp/boot/CODE.BIN of=$@ bs=1 seek=90 skip=90 count=420 conv=notrunc

../build/rootfs/LOADER.SYS: LOADER.ASM
	$(NAS) $(NASMFLAGS) $^ -o $@

../build/tmp/boot/CODE.BIN: MAIN.ASM
	$(NAS) $(NASMFLAGS) $^ -o $@
