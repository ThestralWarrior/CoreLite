#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80
#define WHITE_ON_BLACK 0x0f
#define VGA_CTRL_REGISTER 0x3D4
#define VGA_DATA_REGISTER 0x3D5
#define VGA_OFFSET_HIGH 14
#define VGA_OFFSET_LOW 15

void print_at(char *message, int col, int row);
void print(char *message);
void print_backspace();
void clear_screen();