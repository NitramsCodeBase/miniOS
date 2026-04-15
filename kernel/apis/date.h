#ifndef DATE_H
#define DATE_H

#include "../io.h"

typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;

typedef struct
{
    int day;
    int month;
    int year;
} Date;

Date get_rtc_date(void);
void print_date(void);

#endif