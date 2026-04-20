#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

typedef struct {
    int x1;
    int y1;
    int x2;
    int y2;
} MessageboxCoordinates;

void create_messagebox(MessageboxCoordinates coordinates, const char* caption);

#endif