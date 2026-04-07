#ifndef COMMANDS_H
#define COMMANDS_H

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

void move_cursor();
void clear_screen();
void scroll();
void put_char(char c);
void print(const char* s);
void println(const char* s);
int strcmp(const char* a, const char* b);
int starts_with(const char* str, const char* prefix);
int strlen(const char* s);
void prompt();
void backspace();
char scancode_to_ascii(u8 sc);
void reboot();
void print_number(int num);
void execute_command(const char* cmd);

#endif