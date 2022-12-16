; A boot sector that boots a C kernel in 32-bit protected mode

[org 0x7c00]
KERNEL_OFFSET equ 0x1000
    mov [BOOT_DRIVE], dl ; BIOS stores our boot drive in DL , so it ’s
                         ; best to remember this for later.

    mov bp, 0x9000 ; setup stack
    mov sp, bp

    mov bx, MSG_REAL_MODE   ; notify that booted successfully
    call print_string       ; in 16-bit real mode

    call load_kernel  ; load kernel!!!!

    call switch_to_pm ; switch to protected mode

    jmp $

; Includes
%include "boot/print/print_string.nasm"
%include "boot/disk/disk_load.nasm"
%include "boot/pm/gdt.nasm"
%include "boot/pm/print_string_pm.nasm"
%include "boot/pm/switch_to_pm.nasm"

[bits 16]
load_kernel:
    mov bx, MSG_LOAD_KERNEL ; MWAHAHAHAHAA
    call print_string

    mov bx, KERNEL_OFFSET
    mov dh, 33
    mov dl, [BOOT_DRIVE]
    call disk_load

    ret

[bits 32]
BEGIN_PM:
    mov ebx, MSG_PROT_MODE
    call print_string_pm
    call KERNEL_OFFSET
    jmp $

; Global variables
BOOT_DRIVE      db 0
MSG_STARTED     db "STARTED!", 0
MSG_REAL_MODE   db "Started in 16-bit Real Mode" , 0
MSG_PROT_MODE   db "Successfully landed in 32-bit Protected Mode" , 0
MSG_LOAD_KERNEL db "Loading kernel into memory." , 0
MSG_DEBUG       db "DEBUG!", 0

times 510-($-$$) db 0
dw 0xaa55
