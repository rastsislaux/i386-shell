#include <stdint.h>

#include "screen.h"

#include "../cpu/ports.h"
#include "../cpu/types.h"

#include "../libc/memory.h"
#include "../libc/string.h"

int get_screen_offset(int row, int col){
    return (row * MAX_COLS + col) * 2;
}

int handle_scrolling(int offset) {
    
    // if cursor is within screen, remain unmodified
    if (offset < MAX_ROWS * MAX_COLS * 2) {
        return offset;
    }

    // shuffle the rows back
    memcpy(
        (char*) VIDEO_ADDRESS + 2 * MAX_COLS,
        (char*) VIDEO_ADDRESS,
        2 * MAX_COLS * MAX_ROWS
    );

    char* last_line = get_screen_offset(0, MAX_ROWS - 1) + (char*) VIDEO_ADDRESS;
    for (int i = 0; i < MAX_COLS * 2; i++) {
        last_line[i] = 0;
    }

    offset -= 2 * MAX_COLS;

    return offset;
}

int get_cursor(){
    port_byte_out(REG_SCREEN_CTRL, 14);
    int offset = port_byte_in(REG_SCREEN_DATA) << 8;
    port_byte_out(REG_SCREEN_CTRL, 15);
    offset += port_byte_in(REG_SCREEN_DATA);
    return offset * 2;
}

void set_cursor(int offset){
    offset /= 2;
    port_byte_out(REG_SCREEN_CTRL, 14);
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset >> 8));
    port_byte_out(REG_SCREEN_CTRL, 15);
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset));
}

/* puts char at row&col or at cursor position */
void puts_char(char character, int col, int row, char attribute_byte) {
    unsigned char *video_memory = (unsigned char*) VIDEO_ADDRESS;

    /* if attribute byte is 0, then default style */
    if (!attribute_byte) {
        attribute_byte = WHITE_ON_BLACK;
    }

    /* Get video memory offset for screen loc */
    int offset;
    /* if col and mem aren't negative, use them, else - cursor pos */
    if (col >= 0 && row >= 0) {
        offset = get_screen_offset(row, col);
    } else {
        offset = get_cursor();
    }

    if (character == '\n') {
        int rows = offset / (2 * MAX_COLS);
        offset = get_screen_offset(rows, 79);
    } else {
        video_memory[offset] = character;
        video_memory[offset + 1] = attribute_byte;
    }

    // make scrolling
    offset = handle_scrolling(offset + 2);

    // update cursos pos
    set_cursor(offset);
}

void clear_screen()
{
	int i,j;
	for(i=0; i<MAX_ROWS; i++){
		for(j=0;j<MAX_COLS;j++){
			puts_char(' ', i, j, WHITE_ON_BLACK);
		}
	}
	set_cursor(get_screen_offset(0, 0));
}

void puts_at(char* message, int row, int col){
	if (row >= 0 && col >= 0){
		set_cursor(get_screen_offset(row, col));
	}

	char c;
	int i = 0;
	while((c = message[i++]) != 0){
		puts_char(c, -1, -1, 0);
	}
}

void puts(char *str)
{
	int i;
	for(i=0; str[i]!='\0'; i++)
		puts_char(str[i], -1, -1, 0);
}

void puti(int decimal) {
    char str[64];
    itoa(decimal, str);
    puts(str);
}

void putx(int hex) {
    char str[64];
    htoa(hex, str);
    puts(str);
}

void set_char_at_video_memory(char character, int offset) {
    uint8_t *vidmem = (uint8_t *) VIDEO_ADDRESS;
    vidmem[offset] = character;
    vidmem[offset + 1] = WHITE_ON_BLACK;
}

void puts_backspace() {
    int newCursor = get_cursor() - 2;
    set_char_at_video_memory(' ', newCursor);
    set_cursor(newCursor);
}
