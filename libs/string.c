#include <stdarg.h>
#include "../kernel/io.h"
#include "string.h"

int strlen(const char* s)
{
    int len = 0;
    while (s[len] != '\0') {
        len++;
    }
    return len;
}

void strcpy(char* dest, const char* src)
{
    int i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

void strcat(char* dest, const char* src)
{
    int i = 0;
    int j = 0;

    while (dest[i] != '\0') {
        i++;
    }

    while (src[j] != '\0') {
        dest[i] = src[j];
        i++;
        j++;
    }

    dest[i] = '\0';
}

int strcmp(const char* a, const char* b)
{
    int i = 0;
    while (a[i] && b[i]) {
        if (a[i] != b[i]) {
            return a[i] - b[i];
        }
        i++;
    }
    return a[i] - b[i];
}

// void printf(const char* fmt, ...)
// {
//     va_list args;
//     va_start(args, fmt);

//     while (*fmt)
//     {
//         if (*fmt == '%')
//         {
//             fmt++;

//             if (*fmt == 'd')
//             {
//                 int num = va_arg(args, int);
//                 print_number(num);
//             }
//             else if (*fmt == 's')
//             {
//                 char* str = va_arg(args, char*);
//                 print(str);
//             }
//             else if (*fmt == 'c')
//             {
//                 char c = (char)va_arg(args, int);
//                 put_char(c);
//             }
//         }
//         else
//         {
//             put_char(*fmt);
//         }

//         fmt++;
//     }

//     va_end(args);
// }