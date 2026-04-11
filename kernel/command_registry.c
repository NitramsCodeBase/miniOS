#include "io.h"
#include "system.h"
#include "string.h"
#include "./apis/time.h"
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
    *BOOT_REQUEST_ADDR = BOOT_MODE_GUI;
    reboot();
}

Command commands[] = {
    {"draw1", cmd_draw},
    {"reboot", cmd_reboot},
    {"help", cmd_help}};

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

void call_command_time(const char *args)
{
    print_time();
}

void call_command_color(const char *args)
{
    const char *colors[] =
    {
        "black",        // 0x0
        "blue",         // 0x1
        "green",        // 0x2
        "cyan",         // 0x3
        "red",          // 0x4
        "magenta",      // 0x5
        "brown",        // 0x6
        "lightgray",   // 0x7
        "darkgray",    // 0x8
        "lightblue",   // 0x9
        "lightgreen",  // 0xA
        "lightcyan",   // 0xB
        "lightred",    // 0xC
        "lightmagenta",// 0xD
        "yellow",       // 0xE
        "white"         // 0xF
    };

    boolean found = false;
    char *parameters[2];

    split(args, ' ', parameters);

    

}