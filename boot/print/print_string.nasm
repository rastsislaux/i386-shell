; IN (bx) OUT ()
print_string:
    pusha
    mov ah, 0x0e ; teletype
  print_string_start:
    mov  al, [bx]
    cmp  al, 0 
    je   print_string_done
    int  0x10
    inc  bx
    jmp  print_string_start
  print_string_done:
    mov al, 0x0a ; line feed
    int 0x10
    mov al, 0x0D ; carriage return
    int 0x10
    popa
    ret