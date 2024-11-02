#include "screen.h"
#include "../libc/mem.h"
#include "../cpu/low_level.h"
#include "../cpu/types.h"

/*Private kernel function headers*/
void print_char(char character, int col, int row, char attribute);
int handle_scrolling(int cursor_offset);
int get_offset(int col, int row);
int get_offset_row(int offset);
int get_offset_col(int offset);
void set_cursor_offset(int offset);
int get_cursor_offset();


/****************************************
 *    Public kernel API functions       *
 ****************************************/

// print a message at a specified location
void print_at(char *message, int col, int row) {
    if(col >= 0 && row >= 0) {
        set_cursor_offset(get_offset(col, row));    // cursor is set to offset based on col and row
    }

    int i = 0;
    while(message[i] != 0) {
        print_char(message[i++], col, row, WHITE_ON_BLACK);
        col++;
        if(col >= MAX_COLS) {
            col = 0;
            row++;
        }
    }
}

// print a message at the current cursor position
void print(char *message) {
    print_at(message, -1, -1);
}

void print_backspace() {
    int offset = get_cursor_offset() - 2;
    int row = get_offset_row(offset);
    int col = get_offset_col(offset);
    print_char(0x00, col, row, WHITE_ON_BLACK);
    set_cursor_offset(get_cursor_offset() - 2);
}

void clear_screen() {
    for(int row = 0; row < MAX_ROWS; row++) {
        for(int col = 0; col < MAX_COLS; col++) {
            print_char(' ', col, row, WHITE_ON_BLACK);
        }
    }
    set_cursor_offset(get_offset(0, 0));    // resets the cursor to the top-left
}




/****************************************
 *    Private kernel functions       *
 ****************************************/


// print an individual character at a specified location
void print_char(char character, int col, int row, char attribute) {
    u8 *vidmem = (u8 *) VIDEO_ADDRESS;

    /*set to default attribute if not specified*/
    if(!attribute) {
        attribute = WHITE_ON_BLACK;
    }

    int offset;
    if(col >= 0 && row >= 0) {
        offset = get_offset(col, row);   // get the offset if col and row are non-negative
    } else {
        offset = get_cursor_offset();                  // get the offset of the current cursor position
    }

    /*if character is newline then position towards the end of the line 
        else write it to the video memory*/
    if(character == '\n') {
        int rows = offset / (2 * MAX_COLS);     
        offset = get_offset(79, rows);
    } else {
        vidmem[offset] = character;
        vidmem[offset+1] = attribute;
    }

    /*increase offset by 2 (1 byte for character, 1 byte for attribute)
        then handle scrolling in case characters go beyond screen size*/
    offset += 2;
    offset = handle_scrolling(offset);
    set_cursor_offset(offset);
}

int handle_scrolling(int cursor_offset) {
    if(cursor_offset < MAX_COLS * MAX_ROWS * 2) return cursor_offset;   // no scrolling if cursor offset is within screen limits

    /*copies each row to it's previous row except for the first row*/
    for(int i = 1; i < MAX_ROWS; i++) {
        memory_copy(get_offset(0, i) + VIDEO_ADDRESS,
        get_offset(0, i - 1) + VIDEO_ADDRESS,
        MAX_COLS *2
        );
    }

    /*sets all the characters in the last row to NULL (ASCII 0)*/
    char *last_line = get_offset(0, MAX_ROWS-1) + VIDEO_ADDRESS;
    for(int i = 0; i < MAX_COLS; i++) {
        last_line[i] = 0;
    }

    /*reset the cursor offset back to the starting position of the last row*/
    cursor_offset -= 2 * MAX_COLS;
    return cursor_offset;
}




/*byte offset functions*/

// get the equivalent byte offset from col and row
int get_offset(int col, int row) {
    return ((row * MAX_COLS) + col) * 2;    
}

int get_offset_row(int offset) {
    return offset / (2 * MAX_COLS);
}

int get_offset_col(int offset) {
    return (offset - (get_offset_row(offset) * 2 * MAX_COLS))/2;
}




/*cursor offset functions*/

void set_cursor_offset(int offset) {
    offset /= 2;    // convert from byte offset to character index

    /*select the high byte register
        then write the high byte of the cursor position to the VGA data register*/
    port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
    port_byte_out(VGA_DATA_REGISTER, (u8) (offset >> 8));

    /*select the low byte register
        then write the low byte of the cursor position to the VGA data reister*/
    port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
    port_byte_out(VGA_DATA_REGISTER, (u8) (offset & 0xFF));
}

int get_cursor_offset() {
    /*select the high byte register
        then retrieve the high byte of the cursor position from the VGA data register*/
    port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
    int offset = port_byte_in(VGA_DATA_REGISTER) << 8;

    /*select the low byte register
        then retrieve the low byte register of the cursor position from the VGA data register*/
    port_byte_out(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
    offset +=  port_byte_in(VGA_DATA_REGISTER);

    return offset * 2;  // convert character index to byte offset
}





