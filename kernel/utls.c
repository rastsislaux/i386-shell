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

int strlen(char s[]) {
    int i = 0;
    while (s[i] != '\0') ++i;
    return i;
}

void append(char s[], char n) {
    int len = strlen(s);
    s[len] = n;
    s[len+1] = '\0';
}

bool backspace(char buffer[]) {
    int len = strlen(buffer);
    if (len > 0) {
        buffer[len - 1] = '\0';
        return true;
    } else {
        return false;
    }
}

int strcmp(char s1[], char s2[]) {
    int i;
    for (i = 0; s1[i] == s2[i]; i++) {
        if (s1[i] == '\0') return 0;
    }
    return s1[i] - s2[i];
}
