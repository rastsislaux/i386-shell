# Sable OS

This is a minimal 32-bit operating system designed for i386 architecture.<br>

## Quick start

You'll need:
- GCC Compiler
- ld
- make
- QEMU

To run this OS in QEMU, use `make run`
To build this OS image, use `make all`

## Debug

You can use `gdb` to debug. Run `make debug` and:
```
$ gdb
(gdb) target remote localhost:9999
(gdb) set architecture i386
(gdb) break *0x7c00
(gdb) cont
```
