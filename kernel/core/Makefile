.PHONY: all
all: ../../build/tmp/kernel/int.o

../../build/tmp/kernel/int.o: interrupt/*
	make all -C interrupt/pic
	make all -C interrupt/exception
	

