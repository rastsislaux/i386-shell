#include "utls.h"

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