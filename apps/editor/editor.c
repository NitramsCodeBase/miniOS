#include "editor.h"
#include "../../kernel/io.h"

void run_editor(void)
{
    disable_shell();
    clear_screen();

    // print whole screen
    print_color(0, 0, 80, 25, "blue", "black");

    // print borders up and down
    print_color(0, 0, 80, 1, "white", "black");
    print_color(0, 24, 80, 25, "white", "black");

    boolean running = true;

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
                break;
            }
            default:
            {

                break;
            }
        }
    }
}