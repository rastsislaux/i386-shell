; IN (bx) OUT ()
print_string:
    pusha
  print_string_start:
    mov  al, [bx]
    cmp  al, 0 
    je   print_string_done
    int  0x10
    inc  bx
    jmp  print_string_start
  print_string_done:
    popa
    ret