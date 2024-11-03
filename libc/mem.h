#ifndef MEM_H
#define MEM_H

#include "../cpu/types.h"

void memory_copy(u8* source, u8* dest, int no_of_bytes);
void memory_set(u8* dest, u8 val, u32 length);

#endif