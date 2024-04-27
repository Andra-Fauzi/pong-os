boot_f := src/start.asm
boot_o := start.o
linker := src/linker.ld

all:
	nasm -f elf32 src/start.asm -o start.o
	i686-elf-gcc -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -I./include -c -o kernel.o src/kernel.c
	i686-elf-gcc -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -I./include -c -o screen.o src/screen.c
	i686-elf-gcc -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -I./include -c -o gdt.o src/gdt.c
	i686-elf-gcc -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -I./include -c -o idt.o src/idt.c
	i686-elf-gcc -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -I./include -c -o irs.o src/irs.c
	i686-elf-gcc -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -I./include -c -o irq.o src/irq.c
	i686-elf-gcc -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -I./include -c -o timer.o src/timer.c
	i686-elf-gcc -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -I./include -c -o keyboard.o src/keyboard.c
	ld -m elf_i386 -T src/linker.ld -o kernel.bin start.o kernel.o screen.o gdt.o idt.o irs.o irq.o timer.o keyboard.o
	cp kernel.bin isodir/boot/kernel.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o kernel.iso isodir
	qemu-system-i386 -cdrom kernel.iso

