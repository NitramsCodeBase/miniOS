#include "graphics_api.h"

static u8* framebuffer = 0;
static int screen_width = 0;
static int screen_height = 0;
static int screen_pitch = 0;   // bytes per row

int graphics_enabled = 0;

void graphics_init(u8* fb, int width, int height, int pitch)
{
    framebuffer = fb;
    screen_width = width;
    screen_height = height;
    screen_pitch = pitch;
}

void graphics_enable(void)
{
    graphics_enabled = 1;
}

void graphics_disable(void)
{
    graphics_enabled = 0;
}

void putpixel(int x, int y, u8 color)
{
    if (!framebuffer) return;
    if (x < 0 || y < 0 || x >= screen_width || y >= screen_height) return;

    framebuffer[y * screen_pitch + x] = color;
}

void graphics_clear(u8 color)
{
    if (!framebuffer) return;

    for (int y = 0; y < screen_height; y++)
    {
        for (int x = 0; x < screen_width; x++)
        {
            framebuffer[y * screen_pitch + x] = color;
        }
    }
}

void draw_rect(int x, int y, int w, int h, u8 color)
{
    for (int yy = y; yy < y + h; yy++)
    {
        for (int xx = x; xx < x + w; xx++)
        {
            putpixel(xx, yy, color);
        }
    }
}

void draw_rect_outline(int x, int y, int w, int h, u8 color)
{
    for (int xx = x; xx < x + w; xx++)
    {
        putpixel(xx, y, color);
        putpixel(xx, y + h - 1, color);
    }

    for (int yy = y; yy < y + h; yy++)
    {
        putpixel(x, yy, color);
        putpixel(x + w - 1, yy, color);
    }
}

void draw_mini_screen(void)
{
    graphics_clear(1);                  // blue background
    draw_rect(0, 0, 320, 18, 8);        // top bar
    draw_rect(10, 30, 140, 90, 7);      // light gray window
    draw_rect_outline(10, 30, 140, 90, 15);  // white border

    draw_rect(20, 45, 40, 20, 4);       // red button
    draw_rect(70, 45, 40, 20, 2);       // green button
    draw_rect(20, 75, 100, 30, 3);      // cyan content box
}