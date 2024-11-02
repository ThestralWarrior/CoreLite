#ifndef IDT_H
#define IDT_h

#include "types.h"

#define CS 0x08

/*struct for an idt entry*/
typedef struct {
    u16 base_low;   // lower 16 bits of the handler function address to jump to when interrupt fires
    u16 sel;        // segment selector 
    u8 always0;     // must always be 0
    /*
    * Bit 7: interrupt is present
    * Bits 6 - 5: priviledge level of caller (0 - kernel, 3 - user)
    * Bit 4: set to 0 for interrupt gates
    * Bits 3 - 0: bits 1110 = decimal 14 = "32-bit interrupt gate"
    */
    u8 flags;
    u16 base_high;  // upper 16 bits of the handler function address
} __attribute__((packed)) idt_gate_t;

/*struct to idt*/
typedef struct {
    u16 limit;
    u32 base;
} __attribute__((packed)) idt_register_t;

#define IDT_ENTRIES 256

static idt_gate_t idt[IDT_ENTRIES];
static idt_register_t idt_reg;

void set_idt_gate(int n, u32 handler);
void set_idt();

#endif