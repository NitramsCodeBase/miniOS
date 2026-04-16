#ifndef COMMAND_REGISTRY_H
#define COMMAND_REGISTRY_H

typedef struct {
    const char* name;
    void (*handler)(const char* args);
} Command; 

void execute_command(const char* input);
void cmd_execute_mini();
void call_command_time(const char* args);
void call_command_color(const char* args);
void call_cmd_date(const char* args);
void call_cmd_editor(const char* args);

#endif