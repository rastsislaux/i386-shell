#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>
#include <stdint.h>
#include "../cpu/types.h"

void memcpy(char* source, char* dest, int no_bytes);

void memset(char* dest, unsigned char val, int no_bytes);

uint32_t malloc_int(size_t size, int align, uint32_t* phys_addr); // internal
uint32_t malloc_a(size_t sz);  // page aligned.
uint32_t malloc_p(size_t sz, uint32_t *phys); // returns a physical address.
uint32_t malloc_ap(size_t sz, uint32_t *phys); // page aligned and returns a physical address.
uint32_t malloc(size_t sz); // vanilla (normal).

#endif