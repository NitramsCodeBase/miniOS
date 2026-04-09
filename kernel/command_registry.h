#ifndef COMMAND_REGISTRY_H
#define COMMAND_REGISTRY_H

typedef struct {
    const char* name;
    void (*handler)(const char* args);
} Command; 

void execute_command(const char* input);

#endif