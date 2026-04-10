#ifndef TIME_H
#define TIME_H

typedef unsigned char u8;

typedef struct
{
    u8 hours;
    u8 minutes;
    u8 seconds;
} Time;

int bcd_to_int(u8 bcd);
void print_two_digits(int value);
Time get_rtc_time();
void print_time();

#endif