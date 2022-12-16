C_SOURCES = $(wildcard kernel/*.c drivers/*.c cpu/*.c libc/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h cpu/*.h libc/*.h)
OBJ = ${C_SOURCES:.c=.o cpu/interrupt.o}

CFLAGS = -fno-pic -g -ffreestanding -Wall -Wextra -fno-exceptions  -m32

# Default
all: image.bin

# simulate booting
run: all
	qemu-system-i386 image.bin

debug:
	qemu-system-i386 image.bin -gdb tcp::9999 -S

image.bin: boot/boot_sect.bin kernel.bin
	cat $^ > image.bin

# Build the kernel binary
# $^ is substituted with all of the target’s dependancy files
kernel.bin: kernel/kernel_entry.o ${OBJ}
	ld -m elf_i386 -o $@ -Ttext 0x1000 $^ --oformat binary

# $< is the first dependancy and $@ is the target file
%.o: %.c ${HEADERS}
	gcc ${CFLAGS} -DTOTAL_RAM_SIZE=$(RAM_SIZE) -c $< -o $@

# Assemble assembly code (for kernel_entry)
%.o : %.nasm
	nasm $< -f elf -o $@

# Assemble the boot sector to raw machine code
# The -I options tells nasm where to find our useful assembly
# routines that we include in boot_sect . asm
%.bin : %.nasm
	nasm $< -f bin -I . -o $@

clean :
	rm -fr *.bin *.dis *.o image.bin
	rm -fr kernel/*.o boot/*.bin drivers/*.o cpu/*.o
