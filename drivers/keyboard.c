#include "keyboard.h"
#include "screen.h"
#include "../cpu/isr.h"
#include "../cpu/types.h"
#include "../cpu/low_level.h"
#include "../libc/string.h"

#define SC_MAX 57

#define BACKSPACE 0x0e
#define ENTRY 0x1c

static char key_buffer[256];

const char* sc_name[] = {
    "ERROR", "Esc", "1", "2", "3", "4", "5", "6", 
    "7", "8", "9", "0", "-", "=", "Backspace", "Tab", "Q", "W", "E", 
    "R", "T", "Y", "U", "I", "O", "P", "[", "]", "Enter", "Lctrl", 
    "A", "S", "D", "F", "G", "H", "J", "K", "L", ";", "'", "`", 
    "LShift", "\\", "Z", "X", "C", "V", "B", "N", "M", ",", ".", 
    "/", "RShift", "Keypad *", "LAlt", "Spacebar"
};

const char sc_ascii[] = {
    '?', '?', '1', '2', '3', '4', '5', '6',     
    '7', '8', '9', '0', '-', '=', '?', '?', 'q', 'w', 'e', 'r', 't', 'y', 
    'u', 'i', 'o', 'p', '[', ']', '?', '?', 'a', 's', 'd', 'f', 'g', 
    'h', 'j', 'k', 'l', ';', '\'', '`', '?', '\\', 'z', 'x', 'c', 'v', 
    'b', 'n', 'm', ',', '.', '/', '?', '?', '?', ' '
};

static void keyboard_callback() {
    /*The PIC leaves us the scancode in port 0x60*/
    u8 scancode = port_byte_in(0x60);
    if(scancode > SC_MAX) return;
    if(scancode == BACKSPACE) {
        backspace(key_buffer);
        print_backspace();
    } else if(scancode == ENTRY) {
        print("\n");
//      execute_command(key_buffer);
        key_buffer[0] = '\0';
    } else {
        char ch = sc_ascii[(int) scancode];
        char str[2] = {ch, '\0'};
        append(key_buffer, ch);
        print(str);
    }
}

void init_keyboard() {
    register_interrupt_handler(IRQ1, &keyboard_callback);
}

