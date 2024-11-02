#include "idt.h"

/*setup an interrupt gate for any interrupt number (0 - 31)*/
void set_idt_gate(int n, u32 handler) {
    idt[n].base_low = low_16(handler);
    idt[n].sel = CS;
    idt[n].always0 = 0;
    idt[n].flags = 0x8E;    // 0x8E for 32-bit interrupt gate
    idt[n].base_high = high_16(handler);
}

/*setup the idt pointer and load it*/
void set_idt() {
    idt_reg.base = (u32)&idt;
    idt_reg.limit = (u16) IDT_ENTRIES * sizeof(idt_gate_t) - 1;
    __asm__("lidtl (%0)" : : "r" (&idt_reg));   // load the pointer to idt using lidt command
}