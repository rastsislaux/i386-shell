#ifndef IDT_H
#define IDT_H

#include "types.h"

/* Segment selectors */
#define KERNEL_CS 0x08

/* How every interrupt gate (handler) is defined */
typedef struct {
    uint16_t low_offset;    /* Lower 16 bits of handler function address */
    uint16_t selector;      /* Kernel segment selector */
    uint8_t always0;
    uint8_t flags;          
    uint16_t high_offset;   /* Higher 16 bits of handler function address */
} __attribute__((packed)) idt_gate_t;

/* A pointer to the array of interrupt handlers.
 * Assembly instruction 'lidt' will read it */
typedef struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idt_register_t;

#define IDT_ENTRIES 256

/* Functions implemented in idt.c */
void set_idt_gate(int n, uint32_t handler);
void load_idt();

#endif