#include <stdint.h>

#include "kernel.h"

#include "../cpu/isr.h"
#include "../cpu/timer.h"

#include "../drivers/screen.h"
#include "../drivers/keyboard.h"

#include "../libc/string.h"
#include "../libc/memory.h"
#include "../libc/paging.h"

#include "graphics.h"

void kernel_main() {
    puts(LOGO "\n");
    puts("Installing interrupt service routines (ISRs)...\n");
    isr_install();

    puts("Enabling external interrupts...\n");
    asm volatile("sti");

    puts("Initializing keyboard (IRQ 1)...\n");
    init_keyboard();

    puts("Initializing paging...\n");
    init_paging();
    
    //uint32_t* ptr = (uint32_t*) 0xA000000;
    //uint32_t do_page_fault = *ptr;

    puts("> ");
}

void execute_command(char *input) {
    if (strcmp(input, "EXIT") == 0) {
        puts("Stopping the CPU. Bye!\n");
        asm volatile("hlt");
    }

    if (strcmp(input, "TIMER") == 0) {
        init_timer(100000);
        return;
    }

    if (strcmp(input, "CLEAR") == 0) {
        clear_screen();
        puts("> ");
        return;
    }

    if (strcmp(input, "SABLE") == 0) {
        puts("Glory to Sobol A. M.!\n> ");
        return;
    }

    if (strcmp(input, "SABLEJR") == 0) {
        puts("Glory to Sobol A.M. and Tsiruk V. A.!\n> ");
        return;
    }

    if (strcmp(input, "DINOSAUR") == 0) {
        puts(DINOSAUR);
        puts("\n> ");
        return;
    }

    if (strcmp(input, "TRICERATOPS") == 0) {
        puts(TRICERATOPS);
        puts("\n> ");
        return;
    }

    if (strcmp(input, "PAGE") == 0) {
        uint32_t phys_addr;
        uint32_t page = malloc_int(1000, 1, &phys_addr);
        char page_str[16] = "";
        htoa(page, page_str);
        char phys_str[16] = "";
        htoa(phys_addr, phys_str);

        puts("Page: ");
        puts(page_str);
        puts(", physical address: ");
        puts(phys_str);
        puts("\n");
    }

    puts("Unknown command: ");
    puts(input);
    puts("\n> ");
}