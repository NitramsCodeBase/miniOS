#include "io.h"
#include <string.h>
#include "command_registry.h"
#include "./apis/graphics_api.h"

Command cmd;

void cmd_draw(const char *args)
{
    println("Draw command executed");
}

void cmd_reboot(const char *args)
{
    println("Reboot command executed");
}

void cmd_help(const char *args) 
{
        println("Commands:");
        println("help     - show commands");
        println("clear    - clear screen");
        println("about    - show system info");
        println("echo     - echo text");
        println("mem      - shows memory information");
        println("draw     - draws a square on the screen");
        println("reboot   - reboot machine");
        println("shutdown - shutdown the machine");
}

void cmd_execute_mini() 
{
    println("Starting mini...");
    
    graphics_enable();
    draw_mini_screen();
}

Command commands[] = {
    {"draw1", cmd_draw},
    {"reboot", cmd_reboot},
    {"help", cmd_help}
};

void execute_command(const char *input)
{
    for (int i = 0; i < sizeof(commands); i++)
    {
        Command cmd = commands[i];

        if (starts_with(input, cmd.name))
        {
            const char *args = input + strlen(cmd.name);

            if (*args == ' ')
                args++;

            cmd.handler(args);
            return;
        }
    }

    println("Unknown command");
}