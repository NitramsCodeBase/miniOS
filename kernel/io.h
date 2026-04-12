#ifndef IO_H
#define IO_H

typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;
typedef int boolean;

typedef struct {
    const char *key;
    int value;
} ColorPalette;

typedef struct {
    int x;
    int y;
} Cursor;

#define true 1
#define false 0

#define VGA_MEMORY ((u16*)0xB8000)
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

#define WHITE_ON_BLACK      0x0F
#define WHITE_ON_BLUE       0x1F
#define WHITE_ON_GREEN      0x2F
#define LIGHTGRAY_ON_BLACK  0x07

extern int cursor_row;
extern int cursor_col;
extern u8 color;

extern char input_buffer[128];
extern int input_len;

extern int shift_pressed;

u8 inb(u16 port);
void outb(u16 port, u8 value);
void move_cursor();
void move_cursor_to(int x, int y);
void update_cursor_pos();
Cursor get_cursor_pos();
void clear_screen();
void scroll();
void print_hex(unsigned int value);
void put_char(char c);
void print(const char* s);
void println(const char* s);
int starts_with(const char* str, const char* prefix);
void prompt();
void backspace();
char scancode_to_ascii(u8 sc);
void handle_shift(u8 sc);
void reboot();
void exit_qemu();
void print_number(int num);
int atoi(const char *str);
void read_arguments(const char *args, int out[], int count);
void printf(const char* format, ...);
const char* to_upper(const char* str);
void split(const char *args, char seperator, char *out[]);
int get_color_code(const char *color);
const char* get_color(int color_code);
void set_color(int *color_code);

#endif