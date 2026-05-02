#include "menu.h"
#include "../../kernel\io.h"
#include "../../libs/string.h"

char** options;
int _posx;
int _posy;
int len;
int index = 0;

int get_max_entry_size();
void update_menu();

void create_menu(int posx, int posy, char** entries)
{
    len = sizeof(entries) + 1;

    options = entries;
    _posx = posx;
    _posy = posy;

    if(len == 0)
    {
        println("error: menu is empty");
        return;
    }

    print_color(_posx - 1, _posy, _posx + get_max_entry_size() + 1, _posy + len + 1, "lightgray", "lightgray");

    print_color(0, 0, 0, 0, "lightgray", "black");

    for (int i = 0; i <= len; i++) 
    {
        move_cursor_to(posx, posy++);
        println(entries[i]);
    }

    disable_cursor();
}

void hide_menu(int posx, int posy)
{
    // code here.
}

char* menu_handler()
{
    char* option;

    print_color(0, 0, 0, 0, "green", "white");
    move_cursor_to(_posx, _posy);
    printf("%s", options[0]);

    u8 sc = inb(0x60);

    switch(sc)
    {
        case UP_KEY:
        {
            break;
        }
        case 0xD0:
        {
            update_menu();
            break;
        }
        case RETURN_KEY:
        {
            option = "ret";
            break;
        }
    }

    return option;
}

int get_max_entry_size()
{
    int size;

    for (int i = 0; i <= len; i++) 
    {
        if (i == 0)
            size = strlen(options[i]);

        if(strlen(options[i]) > size)
            size = strlen(options[i]);
    }

    return size;
}

void update_menu()
{
    move_cursor_to(0,0);
    printf("%d", index++);

    return;
    print_color(0, 0, 0, 0, "green", "white");
    move_cursor_to(_posx, _posy + index);
    printf("%s", options[index]);
}