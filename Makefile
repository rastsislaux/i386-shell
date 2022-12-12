C_SOURCES = $(wildcard kernel/*.c drivers/*.c cpu/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h cpu/*.h)
OBJ = ${C_SOURCES:.c=.o cpu/interrupt.o}

# Default
all: os-image

# simulate booting
run: all
	qemu-system-i386 os-image

debug:
	qemu-system-i386 os-image -gdb tcp::9999 -S

os-image: boot/boot_sect.bin kernel.bin
	cat $^ > os-image

# Build the kernel binary
# $^ is substituted with all of the target’s dependancy files
kernel.bin: kernel/kernel_entry.o ${OBJ}
	ld -m elf_i386 -o $@ -Ttext 0x1000 $^ --oformat binary

# $< is the first dependancy and $@ is the target file
%.o: %.c ${HEADERS}
	gcc -fno-pic -m32 -ffreestanding -c $< -o $@

# Assemble assembly code (for kernel_entry)
%.o : %.nasm
	nasm $< -f elf -o $@

# Assemble the boot sector to raw machine code
# The -I options tells nasm where to find our useful assembly
# routines that we include in boot_sect . asm
%.bin : %.nasm
	nasm $< -f bin -I . -o $@
#boot_sect.bin : boot_sect.nasm
#	nasm $< -f bin -I . -o $@

clean :
	rm -fr *.bin *.dis *.o os-image
	rm -fr kernel/*.o boot/*.bin drivers/*.o cpu/*.o
