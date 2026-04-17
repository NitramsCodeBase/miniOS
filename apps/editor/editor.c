#include "editor.h"
#include "../../kernel/io.h"
#include "../../libs/string.h"

boolean running;
Cursor current_cursor_pos;

const char* button_yes  = "  Yes  ";
const char* button_no   = "  No   ";
int button_index        = 1;

void handler(u8 sc);
void terminate_editor_message();
void confirmed_about_dialog(int position, const char* caption);
void show_about();
void hide_about();
boolean close_app_dialog();
void hide_close_app_dialog();
void update_close_dialog_button();

void run_editor(void)
{
    clear_screen();

    input_len = 0;

    // print borders up and down
    print_color(0, 0, 80, 1, "white", "black");
    print_color(0, 24, 80, 25, "white", "black");

    print_color(0, 1, 1, 24, "white", "black");
    print_color(79, 1, 80, 24, "white", "black");

    const char *menu = "File    Options     Help";

    move_cursor_to(2, 0);
    println(menu);

    // print whole screen
    print_color(1, 1, 79, 24, "blue", "black");

    print_color(0, 0, 0, 0, "blue", "white");
    move_cursor_to(1, 1);

    running = true;

    while (running)
    {
        if (!(inb(0x64) & 1))
            continue;

        u8 sc = inb(0x60);

        if (sc & 0x80)
            continue;

        handler(sc);
    }
}

void handler(u8 sc)
{
    Cursor cursor;

    switch (sc)
    {
        case RETURN_KEY:
        {
            put_char('\n');
            input_buffer[input_len] = '\0';
            input_len = 0;

            cursor = get_cursor_pos();
            move_cursor_to(1, cursor.y++);
            break;
        }
        case BACKSPACE_KEY:
        {
            backspace();
            break;
        }
        case UP_KEY:
        {
            cursor = get_cursor_pos();

            if (cursor.y <= 1)
                break;

            move_cursor_to(cursor.x, --cursor.y);
            break;
        }
        case DOWN_KEY:
        {
            cursor = get_cursor_pos();

            if (cursor.y > VGA_HEIGHT - 3)
                break;

            move_cursor_to(cursor.x, ++cursor.y);
            break;
        }
        case LEFT_KEY:
        {
            cursor = get_cursor_pos();

            if (cursor.x <= 1)
                break;

            move_cursor_to(--cursor.x, cursor.y);
            break;
        }
        case RIGHT_KEY:
        {
            cursor = get_cursor_pos();

            if (cursor.x > 79)
                break;

            move_cursor_to(++cursor.x, cursor.y);
            break;
        }
        case ESCAPE_KEY:
        {
            current_cursor_pos = get_cursor_pos();

            running = close_app_dialog();

            move_cursor_to(current_cursor_pos.x, current_cursor_pos.y);

            if(!running) 
            {
                enable_shell();
                terminate_editor_message();
            }
            break;
        }
        case F1_KEY:
        {
            current_cursor_pos = get_cursor_pos();
            show_about();
            break;
        }
        default:
        {
            char c = scancode_to_ascii(sc);

            if (c && input_len >= MAX_COL_WIDTH)
                break;

            if (c && input_len < (int)(sizeof(input_buffer) - 1))
            {
                input_buffer[input_len++] = c;
                put_char(c);
            }
            break;
        }
    }
}

void terminate_editor_message()
{
    move_cursor_to(0, 0);
    println(">editor has been terminated.\n");
}

void show_about() 
{
    boolean running = true;
    disable_cursor();

    // draw about dialog
    print_color(20, 7, 60, 15, "lightgray", "black");
    print_color(20, 6, 60, 7, "white", "black");
    
    const char* title = "about editor";

    move_cursor_to(20, 6);
    println(title);

    // draw dialog shadows
    print_color(21, 15, 61, 16, "black", "black");
    print_color(60, 7, 61, 15, "black", "black");

    print_color(0, 0, 0, 0, "white", "black");

    const char* button = "  Okay  ";
    int x = (strlen(button) + 80) / 2 - strlen(button);

    move_cursor_to(x, 13);
    println(button);

    print_color(0, 0, 0, 0, "lightgray", "black");
    move_cursor_to(21, 8);
    println("text editor program for miniOS");
    move_cursor_to(21, 9);
    println("by Martin Steinkasserer, 2026");

    while(running) 
    {
        if (!(inb(0x64) & 1))
            continue;

        u8 sc = inb(0x60);

        if (sc & 0x80)
            continue;

        switch(sc) 
        {
            case RETURN_KEY:
            {
                running = false;
                confirmed_about_dialog(x, button);
                break;
            }
            default:
            {

                break;
            }
        }
    }

    enable_cursor();
    move_cursor_to(current_cursor_pos.x, current_cursor_pos.y);

    print_color(0, 0, 0, 0, "blue", "white");

    hide_about();
}

void confirmed_about_dialog(int position, const char *caption)
{
    print_color(0, 0, 0, 0, "black", "white");

    move_cursor_to(position, 13);
    println(caption);

    delay_ms(10000);
}

void hide_about()
{
    // draw about dialog
    print_color(20, 7, 60, 15, "blue", "black");
    print_color(20, 6, 60, 7, "blue", "black");
    // draw dialog shadows
    print_color(21, 15, 61, 16, "blue", "black");
    print_color(60, 7, 61, 15, "blue", "black");

    print_color(0, 0, 0, 0, "blue", "white");
}

boolean close_app_dialog()
{
    boolean running = true;
    disable_cursor();

    // draw about dialog
    print_color(20, 7, 60, 15, "lightgray", "black");
    print_color(20, 6, 60, 7, "white", "black");
    
    const char* title = "close editor?";

    move_cursor_to(20, 6);
    println(title);

    // draw dialog shadows
    print_color(21, 15, 61, 16, "black", "black");
    print_color(60, 7, 61, 15, "black", "black");

    print_color(0, 0, 0, 0, "white", "black");

    move_cursor_to(23, 13);
    println(button_yes);

    print_color(0, 0, 0, 0, "black", "white");
    move_cursor_to(57 - strlen(button_no), 13);
    println(button_no);

    while(running)
    {
        if (!(inb(0x64) & 1))
            continue;

        u8 sc = inb(0x60);

        if (sc & 0x80)
            continue;

        switch (sc)
        {
            case RETURN_KEY:
            {
                if(button_index == 1)
                    hide_close_app_dialog();
                
                running = false;
                break;
            }
            case LEFT_KEY:
            {
                if (button_index == 1) 
                    button_index = 0;

                update_close_dialog_button();
                break;
            }
            case RIGHT_KEY:
            {
                if (button_index == 0)
                    button_index = 1;

                update_close_dialog_button();
                break;
            }
            default:
            {

                break;
            }
        }
    }

    enable_cursor();
    move_cursor_to(current_cursor_pos.x, current_cursor_pos.y);

    return button_index == 1;
}

void hide_close_app_dialog()
{
    hide_about();
}

void update_close_dialog_button()
{
    switch(button_index)
    {
        case 0:
        {
            print_color(0, 0, 0, 0, "black", "white");

            move_cursor_to(23, 13);
            println(button_yes);

            print_color(0, 0, 0, 0, "white", "black");
            move_cursor_to(57 - strlen(button_no), 13);
            println(button_no);
            break;
        }
        case 1:
        {
            print_color(0, 0, 0, 0, "white", "black");

            move_cursor_to(23, 13);
            println(button_yes);

            print_color(0, 0, 0, 0, "black", "white");
            move_cursor_to(57 - strlen(button_no), 13);
            println(button_no);
            break;
        }
    }
}