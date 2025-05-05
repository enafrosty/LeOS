#include <stdint.h>
#include "desktop.h"
#include "window.h"

#define DESKTOP_COLOR (color_t){0, 70, 180, 255} 
#define TASKBAR_COLOR (color_t){220, 220, 220, 255} 
#define TASKBAR_HEIGHT 40

void draw_desktop(void) {
    draw_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, DESKTOP_COLOR);
    draw_rect(0, SCREEN_HEIGHT - TASKBAR_HEIGHT, SCREEN_WIDTH, TASKBAR_HEIGHT, TASKBAR_COLOR);
    window_t main_window;
    window_init(&main_window, 100, 100, 400, 300, "Welcome to LeOS");
    main_window.background = (color_t){255, 255, 255, 255};
    window_draw(&main_window);
} 