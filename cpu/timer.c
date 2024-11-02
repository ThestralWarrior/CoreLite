#include "timer.h"
#include "isr.h"
#include "../drivers/screen.h"
#include "../cpu/low_level.h"

int tick = 0;

void timer_callback() {
    tick++;
//    print("Tick\n");
}

void init_timer(u32 freq) {
    // register the timer_callback function
    register_interrupt_handler(IRQ0, &timer_callback);

    u32 divisor = 1193180 / freq;

    // send command byte (set PIT to repeating mode)
    port_byte_out(0x43, 0x36);

    // extract the low and high bytes of the divisor and then send it byte-wise
    u8 l = (u8)divisor & 0xFF;
    u8 h = (u8)((divisor >> 8) & 0xFF);

    // send it byte-wise
    port_byte_out(0x40, l);
    port_byte_out(0x40, h);
}