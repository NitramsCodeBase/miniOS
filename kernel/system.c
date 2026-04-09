#include "system.h"

inline void execute_text_mode()
{
   __asm__ __volatile__ (
    "int $0x10"
    :
    : "a"(0x0003)
    );
}

void execute_graphic_mode()
{
   __asm__ __volatile__ (
    "int $0x10"
    :
    : "a"(0x0013)
    );
}