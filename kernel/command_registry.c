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
        "light_gray",   // 0x7
        "dark_gray",    // 0x8
        "light_blue",   // 0x9
        "light_green",  // 0xA
        "light_cyan",   // 0xB
        "light_red",    // 0xC
        "light_magenta",// 0xD
        "yellow",       // 0xE
        "white"         // 0xF
    };

    boolean found = false;
    int count = sizeof(colors) / sizeof(colors[0]);
    char *parameters[2];

    split(args, ' ', parameters);

    printf("Setting background to %s and foreground to %s\n", parameters[0], parameters[1]);

    for (int i = 0; i < count; i++)
    {
        const char *current_color = colors[i];

        if (strcmp(args, current_color) == 0)
        {
            printf("Color is set to %s\n", current_color);
            found = true;
            break;
        }
    }

    if (!found)
        println("Color is not defined!");
}