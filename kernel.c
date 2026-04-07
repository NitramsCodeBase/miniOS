#include "libs/io.h"
#include "libs/pit.h"

typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;

#define VGA_MEMORY ((u16*)0xB8000)
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define WHITE_ON_BLACK 0x0F

static int cursor_row = 0;
static int cursor_col = 0;
static u8 color = WHITE_ON_BLACK;

static char input_buffer[128];
static int input_len = 0;

static void move_cursor()
{
    u16 pos = (u16)(cursor_row * VGA_WIDTH + cursor_col);

    outb(0x3D4, 0x0F);
    outb(0x3D5, (u8)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (u8)((pos >> 8) & 0xFF));
}

static void clear_screen()
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

static void scroll()
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

static void put_char(char c)
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

static void print(const char* s)
{
    while (*s)
    {
        put_char(*s);
        s++;
    }
}

static void println(const char* s)
{
    print(s);
    put_char('\n');
}

static int strcmp(const char* a, const char* b)
{
    while (*a && *b)
    {
        if (*a != *b)
            return (int)(unsigned char)*a - (int)(unsigned char)*b;
        a++;
        b++;
    }
    return (int)(unsigned char)*a - (int)(unsigned char)*b;
}

static int starts_with(const char* str, const char* prefix)
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

static int strlen(const char* s)
{
    int len = 0;
    while (s[len]) len++;
    return len;
}

static void prompt()
{
    print("> ");
}

static void backspace()
{
    if (input_len <= 0 || cursor_col <= 2)
        return;

    input_len--;
    cursor_col--;
    VGA_MEMORY[cursor_row * VGA_WIDTH + cursor_col] = ((u16)color << 8) | ' ';
    move_cursor();
}

static char scancode_to_ascii(u8 sc)
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

static void reboot()
{
    outb(0x64, 0xFE);
}

static void print_number(int num)
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

static void execute_command(const char* cmd)
{
    if (strcmp(cmd, "help") == 0)
    {
        println("Commands:");
        println("help     - show commands");
        println("clear    - clear screen");
        println("about    - show system info");
        println("echo     - echo text");
        println("mem      - shows memory information");
        println("reboot   - reboot machine");
        println("shutdown - shutdown the machine");
    }
    else if (strcmp(cmd, "clear") == 0)
    {
        clear_screen();
    }
    else if (strcmp(cmd, "about") == 0)
    {
        println("miniOS 32-bit Kernel 1.0");
        println("Protected Mode active");
    }
    else if (starts_with(cmd, "echo "))
    {
        println(cmd + 5);
    }
    else if (strcmp(cmd, "reboot") == 0)
    {
        println("Rebooting...");
        sleep_ms(1000);
        reboot();
    }
    else if (strcmp(cmd, "mem") == 0) 
    {
         int total_kb = 128 * 1024; // 128 MB angenommen
         int used_kb = 1024;        // Kernel + Stack grob
         int free_kb = total_kb - used_kb;

         print("Total RAM: ");
         print_number(total_kb / 1024);
         println(" MB");

         print("Used RAM: ");
         print_number(used_kb / 1024);
         println(" MB");

         print("Free RAM: ");
         print_number(free_kb / 1024);
         println(" MB");
    }
    else if (strlen(cmd) == 0)
    {
    }
    else
    {
        print("Unknown command: ");
        println(cmd);
    }
}

void kernel_main()
{
    clear_screen();

    pit_init(1000);

    println("miniOS 32-bit");
    println("Type 'help' for commands.");
    put_char('\n');
    prompt();

    while (1)
    {
        if (!(inb(0x64) & 1))
            continue;

        u8 sc = inb(0x60);

        if (sc & 0x80)
            continue;

        if (sc == 0x1C)
        {
            put_char('\n');
            input_buffer[input_len] = '\0';
            execute_command(input_buffer);
            input_len = 0;
            prompt();
        }
        else if (sc == 0x0E)
        {
            backspace();
        }
        else
        {
            char c = scancode_to_ascii(sc);
            if (c && input_len < (int)(sizeof(input_buffer) - 1))
            {
                input_buffer[input_len++] = c;
                put_char(c);
            }
        }
    }
}