#ifndef PIT_H
#define PIT_H

typedef unsigned int u32;

extern volatile u32 timer_ticks;
extern volatile u32 timer_frequency;

void pit_init(u32 freq);

void timer_handler(void);

void irq0_handler(void);

void sleep_ms(u32 ms);

#endif