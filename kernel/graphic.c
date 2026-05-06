#include "graphic.h"
#include "../kernel/io.h"

boolean draw_square(int width, int height)
{
    if (width > VGA_WIDTH || height > VGA_HEIGHT)
    {
        println("Error: Your arguments are out of bound.");
        return false;
    }

    clear_screen();

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            move_cursor_to(x, y);

            if ((y == 0 || y == height - 1 && x <= width) || (x == 0 || x == width - 1 && y <= height))
            {
                print("*");
            }
        }
    }

    return true;
}

boolean draw_filled_square(int width, int height)
{
    if (width > VGA_WIDTH || height > VGA_HEIGHT)
    {
        println("Error: Your arguments are out of bound.");
        return false;
    }

    clear_screen();

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            move_cursor_to(x, y);
            print("*");
        }
    }

    return true;
}