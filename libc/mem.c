#include "mem.h"

void memory_copy(u8* source, u8* dest, int no_of_bytes) {
    for(int i = 0; i < no_of_bytes; i++) {
        *(dest + i) = *(source + i);
    }
}

void memory_set(u8* dest, u8 val, u32 length) {
    u8* temp = (u8*) dest;
    for(; len > 0; len--) *temp++ = val;
}