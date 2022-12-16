#include "common.h"

extern void panic(const char *message, const char *file, uint32_t line)
{
    // We encountered a massive problem and have to stop.
    asm volatile("cli"); // Disable interrupts.

    puts("PANIC(");
    puts(message);
    puts(") at ");
    puts(file);
    puts(":");
    puti(line);
    puts("\n");
    // Halt by going into an infinite loop.
    for(;;);
}