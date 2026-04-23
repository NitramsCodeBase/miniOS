#include "date.h"

/*
    CMOS ports
*/
#define CMOS_ADDRESS 0x70
#define CMOS_DATA    0x71

/*
    RTC registers
*/
#define RTC_SECONDS   0x00
#define RTC_MINUTES   0x02
#define RTC_HOURS     0x04
#define RTC_WEEKDAY   0x06
#define RTC_DAY       0x07
#define RTC_MONTH     0x08
#define RTC_YEAR      0x09
#define RTC_STATUS_A  0x0A
#define RTC_STATUS_B  0x0B

/*
    Helper: convert BCD to integer
*/
static int bcd_to_int(u8 bcd)
{
    return ((bcd >> 4) * 10) + (bcd & 0x0F);
}

/*
    Read one CMOS register
*/
static u8 cmos_read(u8 reg)
{
    outb(CMOS_ADDRESS, reg);
    return inb(CMOS_DATA);
}

/*
    RTC updates its values once per second.
    While updating, values may be inconsistent.
*/
static int rtc_is_updating(void)
{
    return cmos_read(RTC_STATUS_A) & 0x80;
}

/*
    Read date safely from RTC
*/
Date get_rtc_date(void)
{
    Date date;
    u8 day, month, year;
    u8 last_day, last_month, last_year;
    u8 status_b;

    /*
        Read until two consecutive reads match
    */
    do
    {
        while (rtc_is_updating());

        day   = cmos_read(RTC_DAY);
        month = cmos_read(RTC_MONTH);
        year  = cmos_read(RTC_YEAR);

        while (rtc_is_updating());

        last_day   = cmos_read(RTC_DAY);
        last_month = cmos_read(RTC_MONTH);
        last_year  = cmos_read(RTC_YEAR);

    } while (day != last_day || month != last_month || year != last_year);

    status_b = cmos_read(RTC_STATUS_B);

    /*
        If bit 2 of register B is 0, values are BCD
    */
    if ((status_b & 0x04) == 0)
    {
        day   = (u8)bcd_to_int(day);
        month = (u8)bcd_to_int(month);
        year  = (u8)bcd_to_int(year);
    }

    date.day   = day;
    date.month = month;

    /*
        RTC usually stores only 2-digit year.
        Adjust as needed for your OS.
    */
    date.year = 2000 + year;

    return date;
}

/*
    Print with leading zeroes
*/
static void print_two_digits(int value)
{
    if (value < 10)
        put_char('0');

    print_number(value);
}

void print_date(void)
{
    Date d = get_rtc_date();

    printf("current date: %04d-%02d-%02d\n", d.year, d.month, d.day);
}