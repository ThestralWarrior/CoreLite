#include "mem.h"

void memory_copy(char* source, char* dest, int no_of_bytes) {
    for(int i = 0; i < no_of_bytes; i++) {
        *(dest + i) = *(source + i);
    }
}