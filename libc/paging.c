#include "paging.h"
#include "common.h"
#include "memory.h"

#include "../drivers/screen.h"
#include "../libc/string.h";
#include "../libc/function.h";

// The kernel's page directory
page_directory_t* kernel_directory = 0;

// The current page directory;
page_directory_t* current_directory = 0;

uint32_t *frames;
uint32_t nframes;

extern uint32_t free_mem_addr;

// Macros used in the bitset algorithms.
#define INDEX_FROM_BIT(a) (a/(8*4))
#define OFFSET_FROM_BIT(a) (a%(8*4))

// function to set a bit in the frames bitset
static void set_frame(uint32_t frame_addr) {
    uint32_t frame = frame_addr / 0x1000;
    uint32_t idx = INDEX_FROM_BIT(frame);
    uint32_t off = OFFSET_FROM_BIT(frame);

    frames[idx] |= (0x1 << off);
}

// Static function to clear a bit in the frames bitset
static void clear_frame(uint32_t frame_addr) {
    uint32_t frame = frame_addr / 0x1000;
    uint32_t idx = INDEX_FROM_BIT(frame);
    uint32_t off = OFFSET_FROM_BIT(frame);

    frames[idx] &= ~(0x1 << off);
}

// Static function to test if a bit is set
static uint32_t test_frame(uint32_t frame_addr) {
    uint32_t frame = frame_addr / 0x1000;
    uint32_t idx = INDEX_FROM_BIT(frame);
    uint32_t off = OFFSET_FROM_BIT(frame);

    return (frames[idx] & (0x1 << off));
}

// Static function to find the first frame
static uint32_t first_frame() {
    uint32_t i, j;
    for (i = 0; i < INDEX_FROM_BIT(nframes); i++) {
        if (frames[i] != 0xFFFFFFFF) { // nothing free, exit early.
            // at least one bit is free here.
            for (j = 0; j < 32; j++) {
                uint32_t toTest = 0x1 << j;
                if (!(frames[i] & toTest)) {
                    return i * 4 * 8 + j;
                }
            }
        }
    }
}

// Function to allocate a frame
void alloc_frame(page_t* page, int is_kernel, int is_writeable) {
    if (page->frame != 0) {
        return; // Frame was already allocated, return straight away.
    } else {
        uint32_t idx = first_frame(); // idx is now the index of the first free frame.
        if (idx == (uint32_t) -1) {
            PANIC("No free frames!");
        }
        set_frame(idx * 0x1000); // this frame is now ours!
        page->present = 1; // Mark it as present.
        page->rw = (is_writeable) ? 1 : 0; // Should the page be writeable?
        page->user = (is_kernel) ? 0 : 1; // Should the page be user-mode?
        page->frame = idx;
    }
}

void free_frame(page_t* page) {
    uint32_t frame;
    if (!(frame = page->frame)) {
        return;
    }

    clear_frame(frame);
    page->frame = 0x0;
}

void init_paging() {
    uint32_t mem_end_page = 0x1000000;

    nframes = mem_end_page / 0x1000;
    frames = (uint32_t*) malloc(INDEX_FROM_BIT(nframes));
    memset(frames, 0, INDEX_FROM_BIT(nframes));

    uint32_t physical;
    kernel_directory = (page_directory_t*) malloc_int(sizeof(page_directory_t), 1, &physical);
    kernel_directory->physicalAddr = physical;
    memset(kernel_directory, 0, sizeof(page_directory_t));
    // current_directory = kernel_directory;

    // We need to identity map (phys addr = virt addr) from
   // 0x0 to the end of used memory, so we can access this
   // transparently, as if paging wasn't enabled.
   // NOTE that we use a while loop here deliberately.
   // inside the loop body we actually change placement_address
   // by calling kmalloc(). A while loop causes this to be
   // computed on-the-fly rather than once at the start.
    int i = 0;
    while (i < free_mem_addr) {
        // Kernel code is readable but not writeable from userspace.
        alloc_frame( get_page(i, 1, kernel_directory), 0, 0);
        i += 0x1000;
    }

    // Register page fault handler
    register_interrupt_handler(14, page_fault);

    // Enable paging
    switch_page_directory(kernel_directory);
}

void switch_page_directory(page_directory_t *dir) {
    current_directory = dir;
    asm volatile("mov %0, %%cr3":: "r"(dir->tablesPhysical));
    uint32_t cr0;

    asm volatile("mov %%cr0, %0": "=r"(cr0));
    cr0 |= 0x80000000;
    asm volatile("mov %0, %%cr0":: "r"(cr0));
}

page_t* get_page(uint32_t addr, int make, page_directory_t* dir) {
    addr /= 0x1000; // addr to idx
    uint32_t table_idx = addr / 1024; // find 
    if (dir->tables[table_idx]) {
        return &dir->tables[table_idx]->pages[addr % 1024];
    } else if (make) {
        uint32_t temp;
        dir->tables[table_idx] = (page_table_t*) malloc_ap(sizeof(page_table_t), &temp);
        memset(dir->tables[table_idx], 0, 0x1000);
        dir->tablesPhysical[table_idx] = temp | 0x7;
        return &dir->tables[table_idx]->pages[addr % 1024];
    } else {
        return 0;
    }
}

void page_fault(registers_t *regs) {
    uint32_t faulting_address;
    asm volatile("mov %%cr2, %0" : "=r" (faulting_address));

    int present = !(regs->err_code & 0x1);
    int rw = regs->err_code & 0x2;
    int us = regs->err_code & 0x4;
    int reserved = regs->err_code & 0x8;
    int id = regs->err_code & 0x10;

    char hex[16];
    htoa(faulting_address, hex);

    puts("Page fault! ( ");
    if (present) { puts("present: "); }
    if (rw) { puts("read-only "); }
    if (us) { puts("user-mode "); }
    if (reserved) { puts("reserved "); }
    puts(") at 0x");
    puts(hex);
    puts("\n");
    PANIC("Page fault.");
}
