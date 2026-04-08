#include "libs/io.h"
#include "libs/string.h"
#include "libs/pit.h"
#include "libs/commands.h"

void kernel_main()
{
    clear_screen();

    pit_init(1000);

    println("miniOS 32-bit");
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
            execute_command(input_buffer);
            input_len = 0;
            prompt();
        }
        else if (sc == 0x0E)
        {
            backspace();
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