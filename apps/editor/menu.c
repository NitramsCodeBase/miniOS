#include "menu.h"
#include "../../kernel\io.h"

void create_menu(int posx, int posy, char** entries)
{
    int len = sizeof(entries) + 1;

    if(len == 0)
    {
        println("error: menu is empty");
        return;
    }

    for (int i = 0; i <= len; i++) 
    {
        move_cursor_to(posx, posy++);
        println(entries[i]);
    }
}

void hide_menu(int posx, int posy)
{
    // code here.
}