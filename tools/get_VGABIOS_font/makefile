.PHONY:all
all:
	-rm mbr run.img font.h mem2font
	cp run.img.bck run.img
	nasm main.asm -o mbr
	gcc main.c -o mem2font
	dd if=mbr of=run.img bs=512 count=1 conv=notrunc
	#echo 'writemem "font.mem" 0x7e00 0x8e00;quit' | bochs
	bochs
	./mem2font
