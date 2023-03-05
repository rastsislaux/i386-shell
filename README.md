# Sable Shell

This is a minimal shell written to be run on i386 processors without any OS.<br>

## Quick start

You'll need:
- GCC Compiler
- ld
- make
- QEMU

To run this shell in QEMU, use `make run`
To build this shell image, use `make all`

## Debug

You can use `gdb` to debug. Run `make debug` and:
```
$ gdb
(gdb) target remote localhost:9999
(gdb) set architecture i386
(gdb) break *0x7c00
(gdb) cont
```
