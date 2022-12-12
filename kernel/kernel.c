#include "kernel.h"

#include "../cpu/isr.h"

#include "../drivers/screen.h"
#include "../drivers/keyboard.h"

void main() {
    print("Glory to Sobol Kernel!\n");
    print("Installing interrupt service routines (ISRs)...\n");
    isr_install();

    print("Enabling external interrupts...\n");
    asm volatile("sti");

    print("Initializing keyboard (IRQ 1)...\n");
    init_keyboard();
    print("> ");
}

void execute_command(char *input) {
    if (strcmp(input, "EXIT") == 0) {
        print("Stopping the CPU. Bye!\n");
        asm volatile("hlt");
    }
    print("Unknown command: ");
    print(input);
    print("\n> ");
}