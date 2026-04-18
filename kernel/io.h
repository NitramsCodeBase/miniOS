#ifndef IO_H
#define IO_H

typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;
typedef int boolean;

typedef struct {
    char *key;
    int value;
} ColorPalette;

typedef struct {
    int x;
    int y;
} Cursor;

#define true 1
#define false 0

#define VGA_MEMORY          ((u16*)0xB8000)
#define VGA_WIDTH           80
#define VGA_HEIGHT          25
#define VGA_COMMAND_PORT    0x3D4
#define VGA_DATA_PORT       0x3D5

#define WHITE_ON_BLACK      0x0F
#define WHITE_ON_BLUE       0x1F
#define WHITE_ON_GREEN      0x2F
#define LIGHTGRAY_ON_BLACK  0x07

#define ESCAPE_KEY          0x01
#define BACKSPACE_KEY       0x0E
#define RETURN_KEY          0x1C
#define TAB_KEY             0x0F
#define F1_KEY              0x3B
#define F2_KEY              0x3C
#define F3_KEY              0x3D
#define F4_KEY              0x3E
#define F5_KEY              0x3F
#define F6_KEY              0x40
#define F7_KEY              0x41
#define F8_KEY              0x42
#define F9_KEY              0x43
#define F10_KEY             0x44

#define UP_KEY              0x48
#define DOWN_KEY            0x50
#define LEFT_KEY            0x4B
#define RIGHT_KEY           0x4D

// Letters
#define A_KEY               0x1E
#define B_KEY               0x30
#define C_KEY               0x2E
#define D_KEY               0x20
#define E_KEY               0x12
#define F_KEY               0x21
#define G_KEY               0x22
#define H_KEY               0x23
#define I_KEY               0x17
#define J_KEY               0x24
#define K_KEY               0x25
#define L_KEY               0x26
#define M_KEY               0x32
#define N_KEY               0x31
#define O_KEY               0x18
#define P_KEY               0x19
#define Q_KEY               0x10
#define R_KEY               0x13
#define S_KEY               0x1F
#define T_KEY               0x14
#define U_KEY               0x16
#define V_KEY               0x2F
#define W_KEY               0x11
#define X_KEY               0x2D
#define Y_KEY               0x2C
#define Z_KEY               0x15

// Numbers (top row)
#define KEY_1               0x02
#define KEY_2               0x03
#define KEY_3               0x04
#define KEY_4               0x05
#define KEY_5               0x06
#define KEY_6               0x07
#define KEY_7               0x08
#define KEY_8               0x09
#define KEY_9               0x0A
#define KEY_0               0x0B

// Control keys
#define ENTER_KEY           0x1C
#define ESCAPE_KEY          0x01
#define BACKSPACE_KEY       0x0E
#define TAB_KEY             0x0F

// Modifiers
#define LEFT_SHIFT_KEY      0x2A
#define RIGHT_SHIFT_KEY     0x36
#define CTRL_KEY            0x1D
#define ALT_KEY             0x38

// Arrow keys (extended scancodes → need 0xE0 prefix handling!)
#define ARROW_UP            0x48
#define ARROW_DOWN          0x50
#define ARROW_LEFT          0x4B
#define ARROW_RIGHT         0x4D

// needs to be addressed
#define F11_KEY             0x45
#define F12_KEY             0x46

#define MAX_COL_WIDTH       70
#define MAX_ROW_HEIGHT      24

extern int cursor_row;
extern int cursor_col;
extern u8 color;

extern char input_buffer[128];
extern int input_len;

extern int shift_pressed;

extern boolean active_app;

u8 inb(u16 port);
void outb(u16 port, u8 value);
void move_cursor();
void move_cursor_to(int x, int y);
void cursor_update();
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
void cprintf(const char* format, char* bg, char* fg, ...);
char* to_upper(char *str);
char* to_lower(char *str);
void split(const char *args, char seperator, char *out[]);
int get_color_code(const char *color);
char* get_color(int color_code);
void set_color(int *color_code);
void set_color_defaults();
const char** get_active_color_scheme(void);
int get_max_color_palette();
void delete_char_at_position(Cursor cur, char ch);
void print_color(int x_start, int y_start, int x_end, int y_end, char* bg, char* fg);
void set_background_color(char* bg_color);
void set_foreground_color(char* fg_color);
void enable_shell();
void disable_shell();
void enable_cursor();
void disable_cursor();
void delay_ms(unsigned int ms);

#endif