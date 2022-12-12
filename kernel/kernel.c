#include "kernel.h"

#include "../cpu/isr.h"

#include "../drivers/screen.h"
#include "../drivers/keyboard.h"

void main() {
    print("Welcome to Sable OS!\n");
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

    if (strcmp(input, "SABLE") == 0) {
        print("Glory to Sobol A. M.!\n> ");
        return;
    }

    if (strcmp(input, "SABLEJR") == 0) {
        print("Glory to Sobol A.M. and Tsiruk V. A.!\n> ");
        return;
    }

    print("Unknown command: ");
    print(input);
    print("\n> ");
}