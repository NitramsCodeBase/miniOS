#include "editor.h"
#include "../../kernel/io.h"

boolean running;

void handler(u8 sc);
void terminate_editor_message();

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
            running = false;
            enable_shell();
            terminate_editor_message();
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
    println("editor has been terminated.\n");
}