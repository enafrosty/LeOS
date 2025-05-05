#ifndef WINDOW_H
#define WINDOW_H

#include <stdint.h>

struct window {
    int x;
    int y;
    int width;
    int height;
    char* title;
    uint32_t* buffer;
};

void init_window(struct window* win, int x, int y, int width, int height, const char* title);
void draw_window(struct window* win);
void move_window(struct window* win, int x, int y);
void resize_window(struct window* win, int width, int height);
void set_window_title(struct window* win, const char* title);

#endif 