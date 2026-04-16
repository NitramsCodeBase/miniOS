#include "mouse.h"
#include "../io.h"

typedef unsigned char  u8;
typedef signed char    s8;
typedef unsigned short u16;

#define VGA_WIDTH   80
#define VGA_HEIGHT  25
// #define VGA_MEMORY  ((volatile u16*)0xB8000)

#define MOUSE_PORT         0x60
#define MOUSE_STATUS_PORT  0x64
#define MOUSE_COMMAND_PORT 0x64

static int mouse_x = VGA_WIDTH / 2;
static int mouse_y = VGA_HEIGHT / 2;

static u8 mouse_cycle = 0;
static u8 mouse_packet[3];

static u16 old_cell = 0;
static int old_x = -1;
static int old_y = -1;

static void mouse_wait_input()
{
    while (inb(MOUSE_STATUS_PORT) & 1)
    {
    }
}

static void mouse_wait_output()
{
    while (!(inb(MOUSE_STATUS_PORT) & 1))
    {
    }
}

static void mouse_write(u8 data)
{
    while (inb(MOUSE_STATUS_PORT) & 2)
    {
    }

    outb(MOUSE_COMMAND_PORT, 0xD4);

    while (inb(MOUSE_STATUS_PORT) & 2)
    {
    }

    outb(MOUSE_PORT, data);
}

static u8 mouse_read()
{
    while (!(inb(MOUSE_STATUS_PORT) & 1))
    {
    }

    return inb(MOUSE_PORT);
}

static void mouse_draw_cursor()
{
    int index;

    if (old_x >= 0 && old_y >= 0)
    {
        VGA_MEMORY[old_y * VGA_WIDTH + old_x] = old_cell;
    }

    index = mouse_y * VGA_WIDTH + mouse_x;
    old_cell = VGA_MEMORY[index];

    // high byte = color, low byte = character
    // here we draw an 'X' as mouse pointer
    VGA_MEMORY[index] = (old_cell & 0xFF00) | 'X';

    old_x = mouse_x;
    old_y = mouse_y;
}

void mouse_update_cursor()
{
    mouse_draw_cursor();
}

void mouse_init()
{
    u8 status;

    mouse_x = VGA_WIDTH / 2;
    mouse_y = VGA_HEIGHT / 2;

    old_x = -1;
    old_y = -1;
    mouse_cycle = 0;

    // Enable auxiliary mouse device
    while (inb(MOUSE_STATUS_PORT) & 2)
    {
    }
    outb(MOUSE_COMMAND_PORT, 0xA8);

    // Enable IRQ12
    while (inb(MOUSE_STATUS_PORT) & 2)
    {
    }
    outb(MOUSE_COMMAND_PORT, 0x20);

    while (!(inb(MOUSE_STATUS_PORT) & 1))
    {
    }
    status = inb(MOUSE_PORT);

    status |= 2;

    while (inb(MOUSE_STATUS_PORT) & 2)
    {
    }
    outb(MOUSE_COMMAND_PORT, 0x60);

    while (inb(MOUSE_STATUS_PORT) & 2)
    {
    }
    outb(MOUSE_PORT, status);

    // Tell mouse to use default settings
    mouse_write(0xF6);
    mouse_read(); // ACK

    // Enable mouse packet streaming
    mouse_write(0xF4);
    mouse_read(); // ACK

    mouse_draw_cursor();
}

void mouse_handler()
{
    u8 status = inb(MOUSE_STATUS_PORT);

    while (status & 1)
    {
        s8 x_move;
        s8 y_move;
        u8 data = inb(MOUSE_PORT);

        if (!(status & 0x20))
        {
            status = inb(MOUSE_STATUS_PORT);
            continue;
        }

        switch (mouse_cycle)
        {
            case 0:
                // bit 3 should always be set in first byte
                if (!(data & 0x08))
                    break;

                mouse_packet[0] = data;
                mouse_cycle++;
                break;

            case 1:
                mouse_packet[1] = data;
                mouse_cycle++;
                break;

            case 2:
                mouse_packet[2] = data;
                mouse_cycle = 0;

                x_move = (s8)mouse_packet[1];
                y_move = (s8)mouse_packet[2];

                mouse_x += x_move;
                mouse_y -= y_move; // mouse Y is inverted

                if (mouse_x < 0)
                    mouse_x = 0;
                if (mouse_y < 0)
                    mouse_y = 0;
                if (mouse_x >= VGA_WIDTH)
                    mouse_x = VGA_WIDTH - 1;
                if (mouse_y >= VGA_HEIGHT)
                    mouse_y = VGA_HEIGHT - 1;

                mouse_draw_cursor();
                break;
        }

        status = inb(MOUSE_STATUS_PORT);
    }
}

int mouse_get_x()
{
    return mouse_x;
}

int mouse_get_y()
{
    return mouse_y;
}