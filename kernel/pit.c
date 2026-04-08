#include "pit.h"
#include "io.h"

volatile u32 timer_ticks = 0;
volatile u32 timer_frequency = 1000;

void pit_init(u32 freq)
{
    if (freq == 0)
        return;

    timer_frequency = freq;

    u32 divisor = 1193182 / freq;

    outb(0x43, 0x36);
    outb(0x40, (u8)(divisor & 0xFF));
    outb(0x40, (u8)((divisor >> 8) & 0xFF));
}

void timer_handler(void)
{
    timer_ticks++;
}

void irq0_handler(void)
{
    timer_handler();
    outb(0x20, 0x20);
}

void sleep_ms(u32 ms)
{
    u32 start = timer_ticks;

    while ((timer_ticks - start) < ms)
    {
        __asm__ volatile ("hlt");
    }
}