#include "editor.h"
#include "../../kernel/io.h"
#include "../../kernel/apis/mouse.h"

void terminate_editor_message();

void run_editor(void)
{
    disable_shell();
    clear_screen();

    // print whole screen
    print_color(1, 1, 79, 24, "blue", "black");

    // print borders up and down
    print_color(0, 0, 80, 1, "white", "black");
    print_color(0, 24, 80, 25, "white", "black");

    print_color(0, 1, 1, 24, "white", "black");
    print_color(79, 1, 80, 24, "white", "black");

    const char* menu = "File    Options     Help";

    move_cursor_to(2, 0);
    printf("%s\n", menu);

    boolean running = true;

    // mouse_init();

    while(running) 
    {
        if (!(inb(0x64) & 1))
            continue;

        u8 sc = inb(0x60);

        if (sc & 0x80)
            continue;

        switch (sc)
        {
            case ESCAPE_KEY:
            {
                running = false;
                enable_shell();
                terminate_editor_message();
                break;
            }
            default:
            {
                // mouse_update_cursor();
                break;
            }
        }
    }
}

void terminate_editor_message()
{
    println("editor has been terminated.\n");
}