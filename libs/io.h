#ifndef IO_H
#define IO_H

typedef unsigned char  u8;
typedef unsigned short u16;

static inline u8 inb(u16 port)
{
    u8 result;
    __asm__ volatile ("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

static inline void outb(u16 port, u8 value)
{
    __asm__ volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

#endif