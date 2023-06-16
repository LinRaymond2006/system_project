.PHONY: all
#remember to update the object file list!
all: ../../build/tmp/kernel/int.o \
	../../build/tmp/kernel/mm.o \
	../../build/tmp/kernel/pm.o

../../build/tmp/kernel/int.o: interrupt/*
	make all -C interrupt/pic
	make all -C interrupt/exception
../../build/tmp/kernel/mm.o: mm/*
	make all -C mm
../../build/tmp/kernel/pm.o: process/*
	make all -C process
