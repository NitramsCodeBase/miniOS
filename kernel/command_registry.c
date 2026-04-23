#include "io.h"
#include "system.h"
#include "string.h"
#include "./apis/time.h"
#include "./apis/date.h"
#include "command_registry.h"
#include "./apis/graphics_api.h"
#include "../apps/editor/editor.h"

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

void call_command_time(const char *args)
{
    print_time();
}

void call_command_color(const char *args)
{
    char *parameters[3];
    int color_code[2];
    int len = sizeof(parameters) / sizeof(parameters[0]);

    split(args, ' ', parameters);  
    
    if(strcmp(parameters[0], "--default") == 0 || strcmp(parameters[0], "d") == 0)
    {
        color_code[0] = get_color_code("black");
        color_code[1] = get_color_code("lightgray");

        set_color(color_code);

        clear_screen();
        println("success: set default colors to bg=black, fg=lightgray");
        
        return;
    }
    else if(strcmp(parameters[0], "--list") == 0 || strcmp(parameters[0], "l") == 0)
    {
        int max_size = get_max_color_palette();

        for (int i = 0; i < max_size; i++) 
            printf("%s\n",get_color(i));

        return;
    }
    else if(strcmp(parameters[0], "--help") == 0 || strcmp(parameters[0], "h") == 0)
    {
        println("available commands:\n");
        println("--list, l               : lists all available colors for shell mode");
        println("--set, s [bg] [fg]      : sets the specified color");
        println("--default, d            : sets the default color scheme back to normal\n");
        println("[bg] = background, [fg] = foreground");
        println("for example: color --set[s] black white\n");
        println("typing command without any parameters will return the current color scheme");

        return;
    }
    else if(strcmp(parameters[0], "--set") == 0 || strcmp(parameters[0], "s") == 0)
    {
        if (strlen(parameters[1]) == 2) 
        {
            println("error: --set is missing parameters.");
            return;
        }

        for (int i = 1; i < len; i++) 
        {
            color_code[i - 1] = get_color_code(parameters[i]);
            
            if (color_code[i - 1] == -1) 
            {
                println("error: invalid color code!");
                return;
            }
        }

        set_color(color_code);
    }
    else
    {
        println("error: invalid parameter");
        return;
    }

    clear_screen();

    printf("success: color set to bg=%s, fg=%s\n", parameters[1], parameters[2]);
}

void call_cmd_date(const char* args)
{
    print_date();
}

void call_cmd_editor(const char* args)
{
    run_editor();
}