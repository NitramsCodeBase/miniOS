#include "menu.h"
#include "../../kernel/io.h"
#include "../../libs/string.h"

char** options;
int _posx;
int _posy;
int len;
int index = 0;

u8 last_sc = 0;

int get_max_entry_size();
void redraw_menu();

void create_menu(int posx, int posy, char** entries)
{
    options = entries;
    _posx = posx;
    _posy = posy;
    index = 0;
    last_sc = 0;

    len = sizeof(entries);

    print_color(
        _posx - 1,
        _posy,
        _posx + get_max_entry_size() + 1,
        _posy + len,
        "darkgray",
        "lightgray"
    );

    redraw_menu();
    disable_cursor();
}

void hide_menu()
{
    print_color(_posx - 1, _posy, _posx + get_max_entry_size() + 1, _posy + len + 1, "blue", "black");
}

char* menu_handler()
{
    u8 sc = inb(0x60);

    // Ignore repeated same key while it is held down
    if (sc == last_sc)
        return "";

    last_sc = sc;

    // Key release scancodes have bit 7 set
    if (sc & 0x80)
    {
        last_sc = 0;
        return "";
    }

    switch (sc)
    {
        case UP_KEY:
            if (index > 0)
                index--;
            else
                index = len - 1;

            redraw_menu();
            break;

        case DOWN_KEY:
            if (index < len - 1)
                index++;
            else
                index = 0;

            redraw_menu();
            break;

        case RETURN_KEY:
            return options[index];
        case ESCAPE_KEY:
            return "ret";
    }

    return "";
}

void redraw_menu()
{
    for (int i = 0; i < len; i++)
    {
        print_color(0, 0, 0, 0, i == index ? "green" : "lightgray", i == index ? "white" : "black");

        move_cursor_to(_posx, _posy + i);
        printf("%s", options[i]);
    }

    print_color(0, 0, 0, 0, "lightgray", "black");
}

int get_max_entry_size()
{
    int size = 0;

    for (int i = 0; i < len; i++)
    {
        int current = strlen(options[i]);

        if (current > size)
            size = current;
    }

    return size;
}