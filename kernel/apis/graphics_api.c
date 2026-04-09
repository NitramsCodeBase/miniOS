#include "graphics_api.h"

static uint32_t* framebuffer = 0;
static int screen_width = 0;
static int screen_height = 0;
static int screen_pitch = 0;   // bytes per row

int graphics_enabled = 0;

void graphics_init(uint32_t* fb, int width, int height, int pitch)
{
    framebuffer = fb;
    screen_width = width;
    screen_height = height;
    screen_pitch = pitch;
}

void graphics_enable()
{
    graphics_enabled = 1;
}

void graphics_disable()
{
    graphics_enabled = 0;
}

void putpixel(int x, int y, uint32_t color)
{
    if (!framebuffer) return;
    if (x < 0 || y < 0 || x >= screen_width || y >= screen_height) return;

    uint32_t* row = (uint32_t*)((uint8_t*)framebuffer + y * screen_pitch);
    row[x] = color;
}

void graphics_clear(uint32_t color)
{
    if (!framebuffer) return;

    for (int y = 0; y < screen_height; y++)
    {
        uint32_t* row = (uint32_t*)((uint8_t*)framebuffer + y * screen_pitch);
        for (int x = 0; x < screen_width; x++)
        {
            row[x] = color;
        }
    }
}

void draw_rect(int x, int y, int w, int h, uint32_t color)
{
    for (int yy = y; yy < y + h; yy++)
    {
        for (int xx = x; xx < x + w; xx++)
        {
            putpixel(xx, yy, color);
        }
    }
}

void draw_mini_screen()
{
    graphics_clear(0x00112233);

    draw_rect(50, 50, 220, 100, 0x00FFAA00);
    draw_rect(80, 80, 80, 80, 0x00FFFFFF);
    draw_rect(200, 120, 120, 60, 0x0000AAFF);
}