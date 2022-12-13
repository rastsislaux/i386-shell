#include <stdint.h>

#include "kernel.h"

#include "../cpu/isr.h"
#include "../cpu/timer.h"

#include "../drivers/screen.h"
#include "../drivers/keyboard.h"

#include "../libc/string.h"
#include "../libc/memory.h"

#include "graphics.h"

void kernel_main() {
    print(LOGO "\n");
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

    if (strcmp(input, "TIMER") == 0) {
        init_timer(100000);
        return;
    }

    if (strcmp(input, "CLEAR") == 0) {
        clear_screen();
        print("> ");
        return;
    }

    if (strcmp(input, "SABLE") == 0) {
        print("Glory to Sobol A. M.!\n> ");
        return;
    }

    if (strcmp(input, "SABLEJR") == 0) {
        print("Glory to Sobol A.M. and Tsiruk V. A.!\n> ");
        return;
    }

    if (strcmp(input, "DINOSAUR") == 0) {
        print(DINOSAUR);
        print("\n> ");
        return;
    }

    if (strcmp(input, "TRICERATOPS") == 0) {
        print(TRICERATOPS);
        print("\n> ");
        return;
    }

    if (strcmp(input, "PAGE") == 0) {
        uint32_t phys_addr;
        uint32_t page = malloc_int(1000, 1, &phys_addr);
        char page_str[16] = "";
        htoa(page, page_str);
        char phys_str[16] = "";
        htoa(phys_addr, phys_str);

        print("Page: ");
        print(page_str);
        print(", physical address: ");
        print(phys_str);
        print("\n");
    }

    print("Unknown command: ");
    print(input);
    print("\n> ");
}