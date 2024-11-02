#include "../drivers/screen.h"
#include "../drivers/keyboard.h"
#include "../cpu/isr.h"
#include "../cpu/timer.h"

void main() {
    isr_install();
    clear_screen();
    __asm__("sti");
    init_timer(50);
    init_keyboard();
}