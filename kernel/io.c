#include "io.h"
#include "string.h"

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

void clear_screen()
{
    for (int y = 0; y < VGA_HEIGHT; y++)
    {
        for (int x = 0; x < VGA_WIDTH; x++)
        {
            VGA_MEMORY[y * VGA_WIDTH + x] = ((u16)color << 8) | ' ';
        }
    }

    cursor_row = 0;
    cursor_col = 0;
    move_cursor();
}

void scroll()
{
    if (cursor_row < VGA_HEIGHT)
        return;

    for (int y = 1; y < VGA_HEIGHT; y++)
    {
        for (int x = 0; x < VGA_WIDTH; x++)
        {
            VGA_MEMORY[(y - 1) * VGA_WIDTH + x] = VGA_MEMORY[y * VGA_WIDTH + x];
        }
    }

    for (int x = 0; x < VGA_WIDTH; x++)
    {
        VGA_MEMORY[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = ((u16)color << 8) | ' ';
    }

    cursor_row = VGA_HEIGHT - 1;
}

void put_char(char c)
{
    if (c == '\n')
    {
        cursor_col = 0;
        cursor_row++;
        scroll();
        move_cursor();
        return;
    }

    if (c == '\r')
    {
        cursor_col = 0;
        move_cursor();
        return;
    }

    VGA_MEMORY[cursor_row * VGA_WIDTH + cursor_col] = ((u16)color << 8) | (u8)c;
    cursor_col++;

    if (cursor_col >= VGA_WIDTH)
    {
        cursor_col = 0;
        cursor_row++;
        scroll();
    }

    move_cursor();
}

void print(const char* s)
{
    while (*s)
    {
        put_char(*s);
        s++;
    }
}

void println(const char* s)
{
    print(s);
    put_char('\n');
}

int starts_with(const char* str, const char* prefix)
{
    while (*prefix)
    {
        if (*str != *prefix)
            return 0;
        str++;
        prefix++;
    }
    return 1;
}

void prompt()
{
    print("> ");
}

void backspace()
{
    if (input_len <= 0 || cursor_col <= 2)
        return;

    input_len--;
    cursor_col--;
    VGA_MEMORY[cursor_row * VGA_WIDTH + cursor_col] = ((u16)color << 8) | ' ';
    move_cursor();
}

char scancode_to_ascii(u8 sc)
{
    switch (sc)
    {
        case 0x02: return '1';
        case 0x03: return '2';
        case 0x04: return '3';
        case 0x05: return '4';
        case 0x06: return '5';
        case 0x07: return '6';
        case 0x08: return '7';
        case 0x09: return '8';
        case 0x0A: return '9';
        case 0x0B: return '0';

        case 0x10: return 'q';
        case 0x11: return 'w';
        case 0x12: return 'e';
        case 0x13: return 'r';
        case 0x14: return 't';
        case 0x15: return 'y';
        case 0x16: return 'u';
        case 0x17: return 'i';
        case 0x18: return 'o';
        case 0x19: return 'p';

        case 0x1E: return 'a';
        case 0x1F: return 's';
        case 0x20: return 'd';
        case 0x21: return 'f';
        case 0x22: return 'g';
        case 0x23: return 'h';
        case 0x24: return 'j';
        case 0x25: return 'k';
        case 0x26: return 'l';

        case 0x2C: return 'z';
        case 0x2D: return 'x';
        case 0x2E: return 'c';
        case 0x2F: return 'v';
        case 0x30: return 'b';
        case 0x31: return 'n';
        case 0x32: return 'm';

        case 0x39: return ' ';
        case 0x0C: return '-';
        case 0x34: return '.';
        default:   return 0;
    }
}

void reboot()
{
    outb(0x64, 0xFE);
}

void print_number(int num)
{
    char buffer[16];
    int i = 0;

    if (num == 0)
    {
        put_char('0');
        return;
    }

    while (num > 0)
    {
        buffer[i++] = '0' + (num % 10);
        num /= 10;
    }

    while (i--)
    {
        put_char(buffer[i]);
    }
}