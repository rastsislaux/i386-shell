#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

void memcpy(char* source, char* dest, int no_bytes);

void memset(char* dest, unsigned char val, int no_bytes);

int strlen(char s[]);

void append(char s[], char n);

bool backspace(char buffer[]);

#endif