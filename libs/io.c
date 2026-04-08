#include "io.h"

int cursor_row = 0;
int cursor_col = 0;
u8 color = WHITE_ON_BLACK;

char input_buffer[128];
int input_len = 0;

void move_cursor()
{
    u16 pos = (u16)(cursor_row * VGA_WIDTH + cursor_col);

    outb(0x3D4, 0x0F);
    outb(0x3D5, (u8)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (u8)((pos >> 8) & 0xFF));
}