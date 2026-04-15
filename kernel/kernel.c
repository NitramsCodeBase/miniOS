#include "io.h"
#include "pit.h"
#include "shell.h"
#include "system.h"
#include "./apis/graphics_api.h"

Cursor cursor;

void cursor_update();

void kernel_main()
{
    clear_screen();

    unsigned char active = *BOOT_ACTIVE_ADDR;

    if (active == BOOT_MODE_GUI) 
    {
        graphics_init((u8*)0xA0000, 320, 200, 320);
        graphics_enable();
        draw_mini_screen();

        while(1) 
        {
            u8 sc = inb(0x60);

            if (sc == 0x01) 
            {
                break;
            }
        }

        *BOOT_REQUEST_ADDR = BOOT_MODE_TEXT;
        *OS_ACTIVE = 1;
        
        reboot();
    }

    pit_init(1000);

    println("miniSHELL");
    println("Type 'help' for commands.");
    put_char('\n');
    prompt();

    cursor_update();
    
    while (1)
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
                cursor_update();

                put_char('\n');
                input_buffer[input_len] = '\0';
                shell_execute(input_buffer);
                input_len = 0;
                prompt();
                break;
            }
            case BACKSPACE_KEY: 
            {
                backspace();
                break;
            }
            case ESCAPE_KEY:
            {
                cursor = get_cursor_pos();

                delete_char_at_position(cursor, '\0');

                input_buffer[input_len];
                move_cursor_to(2, cursor.y);
                break;
            }
            default:
            {
                char c = scancode_to_ascii(sc);
                if (c && input_len < (int)(sizeof(input_buffer) - 1))
                {
                    cursor_update();                    
                    input_buffer[input_len++] = c;
                    put_char(c);
                }
                break;
            }
        }
    }
}

void cursor_update()
{
    cursor = get_cursor_pos();
    move_cursor_to(75, 0);
    printf("%02d:%02d", cursor.x - 2, cursor.y - 2);
    move_cursor_to(cursor.x, cursor.y);
}