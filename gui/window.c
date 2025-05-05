#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "window.h"

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768
#define BITS_PER_PIXEL 32

static uint32_t* framebuffer = 0;

void set_framebuffer(uint32_t* fb) {
    framebuffer = fb;
}

void draw_rect(int x, int y, int width, int height, color_t color) {
    uint32_t pixel_color = (color.a << 24) | (color.r << 16) | (color.g << 8) | color.b;
    
    for (int py = y; py < y + height; py++) {
        for (int px = x; px < x + width; px++) {
            if (px >= 0 && px < SCREEN_WIDTH && py >= 0 && py < SCREEN_HEIGHT) {
                framebuffer[py * SCREEN_WIDTH + px] = pixel_color;
            }
        }
    }
}

void draw_window_border(window_t* window) {
    color_t border_color = {0, 0, 0, 255}; 
    draw_rect(window->x, window->y, window->width, 2, border_color);
    draw_rect(window->x, window->y + window->height - 2, window->width, 2, border_color);
    draw_rect(window->x, window->y, 2, window->height, border_color);
    draw_rect(window->x + window->width - 2, window->y, 2, window->height, border_color);
}

void draw_window_title(window_t* window) {
    color_t title_color = {200, 200, 200, 255}; 
    
    draw_rect(window->x, window->y, window->width, 20, title_color);
    
    // TODO: Draw window title text
}

void window_init(window_t* window, int x, int y, int width, int height, char* title) {
    window->x = x;
    window->y = y;
    window->width = width;
    window->height = height;
    window->title = title;
    window->visible = true;
    window->background = (color_t){255, 255, 255, 255}; 
    window->draw = NULL;
    window->handle_event = NULL;
}

void window_draw(window_t* window) {
    if (!window->visible) return;
    
    draw_rect(window->x, window->y, window->width, window->height, window->background);
    
    draw_window_border(window);
    draw_window_title(window);
    
    if (window->draw) {
        window->draw(window);
    }
}

void window_handle_event(window_t* window, int event) {
    if (window->handle_event) {
        window->handle_event(window, event);
    }
} 