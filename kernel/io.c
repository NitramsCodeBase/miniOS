#include "io.h"
#include "string.h"
#include <stdarg.h>

int cursor_row = 0;
int cursor_col = 0;
u8 color = LIGHTGRAY_ON_BLACK;

char input_buffer[128];
int input_len = 0;

int shift_pressed = 0;

u8 inb(u16 port)
{
    u8 result;
    __asm__ volatile("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

void outb(u16 port, u8 value)
{
    __asm__ volatile("outb %0, %1" : : "a"(value), "Nd"(port));
}

void move_cursor()
{
    u16 pos = (u16)(cursor_row * VGA_WIDTH + cursor_col);

    outb(0x3D4, 0x0F);
    outb(0x3D5, (u8)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (u8)((pos >> 8) & 0xFF));
}

void move_cursor_to(int x, int y)
{
    cursor_col = x;
    cursor_row = y;

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

void print(const char *s)
{
    while (*s)
    {
        put_char(*s);
        s++;
    }
}

void println(const char *s)
{
    print(s);
    put_char('\n');
}

int starts_with(const char *str, const char *prefix)
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
    case 0x02:
        return shift_pressed ? '!' : '1';
    case 0x03:
        return shift_pressed ? '"' : '2';
    case 0x04:
        return shift_pressed ? 0 : '3'; // § erstmal weggelassen
    case 0x05:
        return shift_pressed ? '$' : '4';
    case 0x06:
        return shift_pressed ? '%' : '5';
    case 0x07:
        return shift_pressed ? '&' : '6';
    case 0x08:
        return shift_pressed ? '/' : '7';
    case 0x09:
        return shift_pressed ? '(' : '8';
    case 0x0A:
        return shift_pressed ? ')' : '9';
    case 0x0B:
        return shift_pressed ? '=' : '0';

        // case 0x0C: return shift_pressed ? '?' : 'ß';

    case 0x0D:
        return shift_pressed ? '`' : 0;

    case 0x10:
        return shift_pressed ? 'Q' : 'q';
    case 0x11:
        return shift_pressed ? 'W' : 'w';
    case 0x12:
        return shift_pressed ? 'E' : 'e';
    case 0x13:
        return shift_pressed ? 'R' : 'r';
    case 0x14:
        return shift_pressed ? 'T' : 't';
    case 0x15:
        return shift_pressed ? 'Z' : 'z'; // DE!
    case 0x16:
        return shift_pressed ? 'U' : 'u';
    case 0x17:
        return shift_pressed ? 'I' : 'i';
    case 0x18:
        return shift_pressed ? 'O' : 'o';
    case 0x19:
        return shift_pressed ? 'P' : 'p';

    case 0x1A:
        return shift_pressed ? '*' : '+'; // DE: + und *
    case 0x1B:
        return shift_pressed ? '\'' : '#'; // DE: # und '

    case 0x1E:
        return shift_pressed ? 'A' : 'a';
    case 0x1F:
        return shift_pressed ? 'S' : 's';
    case 0x20:
        return shift_pressed ? 'D' : 'd';
    case 0x21:
        return shift_pressed ? 'F' : 'f';
    case 0x22:
        return shift_pressed ? 'G' : 'g';
    case 0x23:
        return shift_pressed ? 'H' : 'h';
    case 0x24:
        return shift_pressed ? 'J' : 'j';
    case 0x25:
        return shift_pressed ? 'K' : 'k';
    case 0x26:
        return shift_pressed ? 'L' : 'l';

    case 0x27:
        return shift_pressed ? ':' : ';';
    case 0x28:
        return shift_pressed ? '"' : 0; // ä/Ä erstmal ausgelassen
    case 0x29:
        return shift_pressed ? 0 : '^'; // ^ / ° erstmal reduziert

    case 0x2B:
        return shift_pressed ? '>' : '<';
    case 0x2C:
        return shift_pressed ? 'Y' : 'y'; // DE!
    case 0x2D:
        return shift_pressed ? 'X' : 'x';
    case 0x2E:
        return shift_pressed ? 'C' : 'c';
    case 0x2F:
        return shift_pressed ? 'V' : 'v';
    case 0x30:
        return shift_pressed ? 'B' : 'b';
    case 0x31:
        return shift_pressed ? 'N' : 'n';
    case 0x32:
        return shift_pressed ? 'M' : 'm';

    case 0x33:
        return shift_pressed ? ';' : ',';
    case 0x34:
        return shift_pressed ? ':' : '.';
    case 0x35:
        return shift_pressed ? '_' : '-';

    case 0x39:
        return ' ';
    case 0x1C:
        return '\n';
    case 0x0E:
        return '\b';
    case 0x0F:
        return '\t';

    default:
        return 0;
    }
}

void handle_shift(u8 sc)
{
    // Shift gedrückt
    if (sc == 0x2A || sc == 0x36)
        shift_pressed = 1;

    // Shift losgelassen
    if (sc == 0xAA || sc == 0xB6)
        shift_pressed = 0;
}

void reboot()
{
    outb(0x64, 0xFE);
}

void exit_qemu()
{
    __asm__ __volatile__(
        "movw $0x2000, %ax \n\t"
        "movw $0x604, %dx  \n\t"
        "outw %ax, %dx");
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

int atoi(const char *str)
{
    int result = 0;

    while (*str >= '0' && *str <= '9')
    {
        result = result * 10 + (*str - '0');
        str++;
    }

    return result;
}

void read_arguments(const char *args, int out[], int count)
{
    const char *p = args;

    for (int i = 0; i < count; i++)
    {
        // Zahl lesen
        int value = 0;

        while (*p == ' ')
            p++;

        while (*p >= '0' && *p <= '9')
        {
            value = value * 10 + (*p - '0');
            p++;
        }

        out[i] = value;
    }
}

void printf(const char *format, ...)
{
    va_list args;
    va_start(args, format);

    while (*format)
    {
        if (*format == '%')
        {
            format++;

            if (*format == 'd')
            {
                int value = va_arg(args, int);
                print_number(value);
            }
            else if (*format == 's')
            {
                char *str = va_arg(args, char *);
                print(str);
            }
            else if (*format == 'c')
            {
                char c = (char)va_arg(args, int);
                put_char(c);
            }
            else
            {
                put_char('%');
                put_char(*format);
            }
        }
        else
        {
            put_char(*format);
        }

        format++;
    }

    va_end(args);
}

void split(const char *args, char seperator, char *out[])
{
    char buf[10][64];
    int i = 0, j = 0, k = 0, p = 0;

    while (args[i] && k < 10)
    {
        while (args[i] == seperator)
            i++;

        if (!args[i])
            break;

        out[k] = buf[k];
        j = 0;

        while (args[i] && args[i] != seperator && j < 63)
            buf[k][j++] = args[i++];

        buf[k++][j] = 0;
    }

    out[k] = 0;
}