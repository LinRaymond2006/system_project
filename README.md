# about the project

I'm a operating system enthusiast, this is my operating system project.

# project structure

- ./
    - bootloader: MBR and second stage loader
    - build: the directory where the output file is stored
    - kernel: kernel source code
        - core: essential parts of the kernel
        - def: some standard definitions
        - stdlib: function that may be useful to the kernel
    - mount_dir: where the rootfs image is mounted
    - run: the bochs emulator configuration, BIOS and VGA firmware binary
    - tools: some tools I've created

# prerequisites installation

install these necessities on your computer to compile/test the system

1.  a linux-based system
2.  the Netwide assembler (nasm)
3.  a compiler, linker, and a assembler (i'm using gcc, ld, and as here)
4.  the "make" tool
5.  dosfstools (mkfs.fat)
6.  objcopy
7.  bochs emulator (if you wish to test the system on bochs vm)

one-liner installation on debian-based distros (some package installed by default):

`apt install nasm gcc binutils make dosfstools bochs*`

# build instructions

build the rootfs image:

`make all`

build the rootfs image and run with bochs emulator:

`make run`

clear the built binary:

`make cleanup`

by the way, the built kernel binary and other files could be found at `build/rootfs`.Since it's a FAT32-formatted image that is generated, same file could also be located in the root file system image

# progress

here's the list of finished/unfinished stuff

## finished

- write a first/second stage bootstrap program work on legacy BIOS
- retrieve VBE inforation from BIOS
- retrieve memory map with BIOS function e820
- basic exception handling
- 8259A pic (basic features)

## things to do next

- memory management using slab

## in progress

- context switching and pcb structure

## something to fix

- the hard-coded SVGA number and screen resolution that only work on specific display cards
- keyboard scaning system
    - choose keyboard set
    - the scancode is incorrect (maybe?)
- the naming convention in the project is inconsistent