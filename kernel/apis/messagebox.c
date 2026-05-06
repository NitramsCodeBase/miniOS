#include "../io.h"
#include "../string.h"
#include "messagebox.h"

Coordinates coords;
enum Button button;

const char* buttons[] = 
{
    "  OKAY  \0",
    "  YES   \0",
    "  NO    \0",
    "  CANCEL\0"
};

void create_messagebox(int x1, int y1, int x2, int y2, enum Button button, const char* caption, const char* content)
{
    disable_cursor();

    coords.x1 = x1;
    coords.y1 = y1;
    coords.x2 = x2;
    coords.y2 = y2;

    print_color(x1, y1, x2, y2, "lightgray", "black");

    print_color(x1, y1, x2, y1 + 1, "white", "black");
    move_cursor_to(x1, y1);
    println(caption);

    int len = 0;

    switch(button)
    {
        case OKAY:
        {
            len = x2 - strlen(buttons[0]) - 2;

            move_cursor_to(len, y2 - 2);
            print(buttons[0]);
            break;
        }
        default:
            break;
    }

    len = strlen(content);

    int x = 2;
    int y = 2;

    print_color(0, 0, 0, 0, "lightgray", "black");

    for(int i = 0; i < len; i++)
    {
        if (content[i] != '\n') 
        {
            move_cursor_to(x1 + x++, y1 + y);
            printf("%c", content[i]);
        }
        else
        {
            x = 2;
            y++;
        }
    }
}

void destroy_messagebox(char* bg, char* fg)
{
    print_color(coords.x1, coords.y1, coords.x2, coords.y2, bg, fg);
    enable_cursor();
}