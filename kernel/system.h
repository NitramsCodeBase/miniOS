#ifndef SYSTEM_H
#define SYSTEM_H

#define BOOT_REQUEST_ADDR ((volatile unsigned char*)0x0500)
#define BOOT_ACTIVE_ADDR  ((volatile unsigned char*)0x0501)
#define BOOT_MODE_TEXT 0
#define BOOT_MODE_GUI  1

void execute_text_mode();
void execute_graphic_mode();

#endif