#include "../io.h"
#include "messagebox.h"

void create_messagebox(MessageboxCoordinates coordinates, const char* caption)
{
    disable_cursor();

    print_color(coordinates.x1, coordinates.y1, coordinates.x2, coordinates.y2, "lightgray", "black");
    move_cursor_to(coordinates.x1, coordinates.y1);

    println(caption);
}