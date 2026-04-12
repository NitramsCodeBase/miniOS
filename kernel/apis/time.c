#include "time.h"
#include "../io.h"

static u8 rtc_read(u8 reg)
{
    outb(0x70, reg);
    return inb(0x71);
}

static int rtc_is_updating()
{
    outb(0x70, 0x0A);
    return inb(0x71) & 0x80;
}

int bcd_to_int(u8 bcd)
{
    return ((bcd >> 4) * 10) + (bcd & 0x0F);
}

void print_two_digits(int value)
{
    if (value < 10)
        print("0");

    print_number(value);
}

Time get_rtc_time()
{
    Time t;
    u8 status_b;
    u8 hours_raw;
    u8 minutes_raw;
    u8 seconds_raw;

    while (rtc_is_updating())
    {
    }

    seconds_raw = rtc_read(0x00);
    minutes_raw = rtc_read(0x02);
    hours_raw   = rtc_read(0x04);
    status_b    = rtc_read(0x0B);

    if ((status_b & 0x04) == 0)
    {
        seconds_raw = (u8)bcd_to_int(seconds_raw);
        minutes_raw = (u8)bcd_to_int(minutes_raw);
        hours_raw   = (u8)bcd_to_int(hours_raw & 0x7F) | (hours_raw & 0x80);
    }

    if ((status_b & 0x02) == 0 && (hours_raw & 0x80))
    {
        hours_raw = ((hours_raw & 0x7F) + 12) % 24;
    }

    t.hours = hours_raw & 0x7F;
    t.minutes = minutes_raw;
    t.seconds = seconds_raw;

    return t;
}

void print_time()
{
    Time t = get_rtc_time();
    
    int hours = (t.hours + 2) % 24;

    print("current time: ");
    print_two_digits(hours);
    print(":");
    print_two_digits(t.minutes);
    print(":");
    print_two_digits(t.seconds);
    println("");
}