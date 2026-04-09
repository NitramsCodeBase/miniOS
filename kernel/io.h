#ifndef IO_H
#define IO_H

typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;

#define VGA_MEMORY ((u16*)0xB8000)
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define WHITE_ON_BLACK 0x0F

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
void clear_screen();
void scroll();
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

#endif