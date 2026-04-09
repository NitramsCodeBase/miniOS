#include "graphic.h"
#include "../kernel/io.h"

void draw_square()
{
    clear_screen();

    for (int y = 0; y < 5; y++)
    {
        for (int x = 0; x < 5; x++)
        {
            move_cursor_to(x, y);
            print("*");
        }
    }
}

// void draw_square(int size)
// {
//     clear_screen();

//     for (int y = 0; y < size; y++)
//     {
//         for (int x = 0; x < size; x++)
//         {
//             move_cursor_to(x, y);

//             if (x == 0 || x == size - 1 || y == 0 || y == size - 1)
//             {
//                 print("*");
//             }
//             else
//             {
//                 print(" ");
//             }
//         }
//     }
// }