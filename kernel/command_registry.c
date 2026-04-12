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
    char *parameters[2];
    int color_code[2];
    split(args, ' ', parameters);  
    
    if(strcmp(parameters[0], "default") == 0)
    {
        color_code[0] = 0;
        color_code[1] = 7;

        set_color(color_code);
        
        clear_screen();
        println("set default colors");
        return;
    }

    int len = sizeof(parameters) / sizeof(parameters[0]);

    if(strcmp(parameters[0], "list") == 0)
    {
        for (int i = 0; i < 16; i++) 
            printf("%s%d. %s\n", i + 1 < 10 ? "0" : "" , i + 1, get_color(i));

        return;
    }

    if(strcmp(parameters[0], "help") == 0)
    {
        println("available commands : list, default");
        println("");
        println("list               : lists all available colors for shell mode");
        println("default            : sets the default color scheme back to normal");
        println("");

        return;
    }

    boolean invalid_color = false;

    for (int i = 0; i < len; i++) 
        color_code[i] = get_color_code(parameters[i]);

    if (color_code[0] == -1 || color_code[1] == -1) 
    {
        println("error: invalid color code!");
        return;
    }

    set_color(color_code);

    clear_screen();

    printf("color set to %s/%s\n", get_color(color_code[0]), get_color(color_code[1]));
}