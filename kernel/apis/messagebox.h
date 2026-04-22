#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

typedef struct {
    int x1;
    int y1;
    int x2;
    int y2;
} Coordinates;

enum Button {
    OKAY,
    CANCEL,
    OKAYCANCEL
};

void create_messagebox(int x1, int y1, int x2, int y2, const char* caption);
void destroy_messagebox(char* bg, char* fg);

#endif