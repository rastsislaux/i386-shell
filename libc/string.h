#ifndef STRINGS_H
#define STRINGS_H

#include <stdbool.h>

int strlen(char s[]);

void strncat(char s[], char n);

bool backspace(char buffer[]);

void htoa(int n, char* str);

void itoa(int n, char* str);

void strrev(char s[]);

int strcmp(char s1[], char s2[]);

#endif