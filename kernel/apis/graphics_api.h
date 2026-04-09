#ifndef GRAPHICS_API_H
#define GRAPHICS_API_H

#include <stdint.h>

extern int graphics_enabled;

void graphics_init(uint32_t* fb, int width, int height, int pitch);
void graphics_enable();
void graphics_disable();

void graphics_clear(uint32_t color);
void putpixel(int x, int y, uint32_t color);
void draw_rect(int x, int y, int w, int h, uint32_t color);
void draw_mini_screen();

#endif