#include "io.h"
#include "commands.h"
#include "../libs/graphic.h"
#include "../libs/string.h"

void execute_command(const char* cmd)
{
    if (strcmp(cmd, "help") == 0)
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
    else if (strcmp(cmd, "clear") == 0 || strcmp(cmd, "cls") == 0)
    {
        clear_screen();
    }
    else if (starts_with(cmd, "draw ")) 
    {
        const char *arguments = cmd + 5;
        
        draw_square();
    }
    else if (strcmp(cmd, "about") == 0 || strcmp(cmd, "ver") == 0)
    {
        println("miniOS 32-bit Kernel 1.0");
        println("Protected Mode active");
        println("Developed by Martin Steinkasserer, for testing purposes.");
    }
    else if (starts_with(cmd, "echo "))
    {
        const char *message = cmd + 5;
        println(message);
    }
    else if (strcmp(cmd, "reboot") == 0)
    {
        println("Rebooting...");
        reboot();
    }
    else if (strcmp(cmd, "shutdown") == 0 || strcmp(cmd, "exit") == 0)
    {
        println("Shutting down...");
        exit_qemu();
    }
    else if (strcmp(cmd, "mem") == 0) 
    {
         int total_kb = 128 * 1024; // 128 MB angenommen
         int used_kb = 1024;        // Kernel + Stack grob
         int free_kb = total_kb - used_kb;

         print("Total RAM: ");
         print_number(total_kb / 1024);
         println(" MB");

         print("Used RAM: ");
         print_number(used_kb / 1024);
         println(" MB");

         print("Free RAM: ");
         print_number(free_kb / 1024);
         println(" MB");
    }
    else
    {
        if(strcmp(cmd, "") == 0)
            return;

        char buffer[256];
        strcpy(buffer, "Unknown command: ");
        strcat(buffer, cmd);

        println(buffer);
    }
}