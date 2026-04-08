#include "commands.h"
#include "io.h"
#include "string.h"

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
        println("reboot   - reboot machine");
        println("shutdown - shutdown the machine");
    }
    else if (strcmp(cmd, "clear") == 0)
    {
        clear_screen();
    }
    else if (strcmp(cmd, "about") == 0)
    {
        println("miniOS 32-bit Kernel 1.0");
        println("Protected Mode active");
    }
    else if (starts_with(cmd, "echo "))
    {
        println(cmd + 5);
    }
    else if (strcmp(cmd, "reboot") == 0)
    {
        println("Rebooting...");
        reboot();
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