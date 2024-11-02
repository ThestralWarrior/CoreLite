#include "string.h"

void int_to_ascii(int n, char str[]) {
    int i = 0, sign;
    if((sign = n) < 0) n = -n;
    do {
        str[i++] = n % 10 + '0';
    } while((n /= 10) > 0);
    if(sign < 0) str[i++] = '-';
    str[i] = '\0';
    reverse(str);
}

void reverse(char s[]) {
    int length = strlen(s);
    for(int i = 0, j = length - 1; i < j; i++, j--) {
        char temp = s[i];
        s[i] = s[j];
        s[j] = temp;
    }
}

int strlen(char s[]) {
    int i = 0;
    while(s[i] != '\0') i++;
    return i;
}

void append(char str[], char ch) {
    int length = strlen(str);
    str[length] = ch;
    str[length + 1] = '\0';
}

void backspace(char str[]) {
    int length = strlen(str);
    str[length - 1] = '\0';
}