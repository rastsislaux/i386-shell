#include <stdint.h>

#include "memory.h"

void memcpy(char* source, char* dest, int no_bytes) {
    for (int i = 0; i < no_bytes; i++) {
        dest[i] = source[i];
    }
}

void memset(char* dest, unsigned char val, int no_bytes) {
    for (int i = 0; i < no_bytes; i++) {
        dest[i] = val;
    }
}

uint32_t free_mem_addr = 0x1000;

uint32_t malloc_int(size_t size, int align, uint32_t* phys_addr) {
    if (align == 1 && (free_mem_addr & 0xFFFFF000)) { // If the address is not already page-aligned
        free_mem_addr &= 0xFFFFF000;
        free_mem_addr += 0x1000;
    }

    if (phys_addr) *phys_addr = free_mem_addr;

    uint32_t ret = free_mem_addr;
    free_mem_addr += size;
    return ret;
}

uint32_t malloc_a(size_t sz)
{
    return malloc_int(sz, 1, 0);
}

uint32_t malloc_p(size_t sz, uint32_t *phys)
{
    return malloc_int(sz, 0, phys);
}

uint32_t malloc_ap(size_t sz, uint32_t *phys)
{
    return malloc_int(sz, 1, phys);
}

uint32_t malloc(size_t sz)
{
    return malloc_int(sz, 0, 0);
}
