#ifndef GRAPHICS_API_H
#define GRAPHICS_API_H

typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;

extern int graphics_enabled;

void graphics_init(u8* fb, int width, int height, int pitch);
void graphics_enable(void);
void graphics_disable(void);

void putpixel(int x, int y, u8 color);
void graphics_clear(u8 color);
void draw_rect(int x, int y, int w, int h, u8 color);
void draw_rect_outline(int x, int y, int w, int h, u8 color);
void draw_mini_screen(void);

#endif