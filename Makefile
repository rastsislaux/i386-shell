# Default
all: os-image

# simulate booting
run: all
	qemu-system-i386 os-image

debug:
	qemu-system-i386 os-image -gdb tcp::9999 -S

os-image: boot_sect.bin kernel.bin
	cat $^ > os-image

# Build the kernel binary
# $^ is substituted with all of the target’s dependancy files
kernel.bin: kernel_entry.o kernel.o
	ld -m elf_i386 -o kernel.bin -Ttext 0x1000 $^ --oformat binary

# Build kernel object file
# $< is the first dependancy and $@ is the target file
kernel.o : kernel.c
	gcc -fno-pic -m32 -ffreestanding -c $< -o $@

# Build kernel entry object file
kernel_entry.o : kernel_entry.nasm
	nasm $< -f elf -o $@

# Assemble the boot sector to raw machine code
# The -I options tells nasm where to find our useful assembly
# routines that we include in boot_sect . asm
boot_sect.bin : boot_sect.nasm
	nasm $< -f bin -I . -o $@

clean :
	rm -fr *.bin *.dis *.o os-image *.map

# Disassemble our kernel - might be useful for debugging .
kernel.dis : kernel.bin
	ndisasm -b 32 $< > $@

os-image.dis : os-image
	ndisasm -b 32 $< > $@