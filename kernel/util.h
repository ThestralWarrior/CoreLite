/*
* util.c and util.h are redundant and should not be used
* refer the C standard library functions defined in libc instead
*/


#ifndef UTIL_H
#define UTIL_H

#include "../cpu/types.h"

void memory_copy(char* source, char* dest, int no_of_bytes);
void int_to_ascii(int n, char str[]);
void reverse(char s[]);
int strlen(char s[]);
void append(char str[], char ch);
void backspace(char str[]);

#endif