#include "io.h"
#include "pit.h"
#include "shell.h"
#include "system.h"
#include "./apis/graphics_api.h"

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
    
    while (1)
    {
        if (!(inb(0x64) & 1))
            continue;

        u8 sc = inb(0x60);

        if (sc & 0x80)
            continue;

        if (sc == 0x1C)
        {
            put_char('\n');
            input_buffer[input_len] = '\0';
            shell_execute(input_buffer);
            input_len = 0;
            prompt();
        }
        else if (sc == 0x0E)
        {
            backspace();
        }
        else if (sc == 0x01) 
        {
            Cursor cur = get_cursor_pos();
            
            input_len += 2;

            for (int i = 2; i < input_len; i++) 
            {
                move_cursor_to(i, cur.y);
                put_char('\0');
            }
            
            input_len = 0;
            input_buffer[input_len];
            
            move_cursor_to(2, cur.y);
        }
        else
        {
            char c = scancode_to_ascii(sc);
            if (c && input_len < (int)(sizeof(input_buffer) - 1))
            {
                input_buffer[input_len++] = c;
                put_char(c);
            }
        }
    }
}