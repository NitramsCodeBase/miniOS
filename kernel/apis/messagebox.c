#include "../io.h"
#include "messagebox.h"

Coordinates coords;
enum Button button;

void create_messagebox(int x1, int y1, int x2, int y2, const char* caption)
{
    disable_cursor();

    coords.x1 = x1;
    coords.y1 = y1;
    coords.x2 = x2;
    coords.y2 = y2;

    print_color(x1, y1, x2, y2, "lightgray", "black");
    move_cursor_to(x1, y1);
    println(caption);
}

void destroy_messagebox(char* bg, char* fg)
{
    print_color(coords.x1, coords.y1, coords.x2, coords.y2, bg, fg);
    enable_cursor();
}