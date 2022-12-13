#include <stdint.h>

#include "keyboard.h"
#include "screen.h"

#include "../cpu/isr.h"
#include "../cpu/ports.h"
#include "../kernel/kernel.h"

#include "../libc/string.h"
#include "../libc/function.h"

#define SC_MAX 57
#define BACKSPACE 0x0E
#define ENTER 0x1C

static char key_buffer[256];

const char scancode_to_char[] = {
  '?', '?', '1', '2', '3', '4', '5',
  '6', '7', '8', '9', '0', '-', '=',
  '?', '?', 'Q', 'W', 'E', 'R', 'T',
  'Y', 'U', 'I', 'O', 'P', '[', ']',
  '?', '?', 'A', 'S', 'D', 'F', 'G',
  'H', 'J', 'K', 'L', ';', '\'', '`',
  '?', '\\', 'Z', 'X', 'C', 'V', 'B',
  'N', 'M', ',', '.', '/', '?', '?',
  '?', ' '
};

static void keyboard_callback(registers_t *regs) {
    uint8_t scancode = port_byte_in(0x60);
    if (scancode > SC_MAX) return;

    if (scancode == BACKSPACE) {
        if (backspace(key_buffer)) {
            print_backspace();
        }
        return;
    }
    
    if (scancode == ENTER) {
        print("\n");
        execute_command(key_buffer);
        key_buffer[0] = 0;
        return;
    }
    
    char letter = scancode_to_char[(int) scancode];
    strncat(key_buffer, letter);
    char str[2] = {letter, '\0'};
    print(str);

    UNUSED(regs);
}

void init_keyboard() {
    register_interrupt_handler(IRQ1, keyboard_callback);
}
