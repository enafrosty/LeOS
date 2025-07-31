#include <stdint.h>
#include <stddef.h>
#include "boot.h"
#include "../gui/desktop.h"

void kernel_main(uint32_t magic, uint32_t boot_info);
void set_framebuffer(uint32_t* fb);
void draw_desktop(void);


/*
 * Place the multiboot header in the dedicated ".multiboot" section so that
 * GRUB can detect it. Without this, the loader will return to the menu as it
 * cannot find a valid multiboot header within the first 8KiB of the kernel.
 */
__attribute__((section(".multiboot"), aligned(4), used))
const struct linux_boot_header boot_header = {
    .magic = LINUX_BOOT_MAGIC,
    .flags = LINUX_BOOT_FLAG_MEMORY_INFO | LINUX_BOOT_FLAG_VIDEO_MODE,
    .checksum = -(LINUX_BOOT_MAGIC + (LINUX_BOOT_FLAG_MEMORY_INFO | LINUX_BOOT_FLAG_VIDEO_MODE)),
    .header_addr = 0x100000,
    .load_addr = 0x100000,
    .load_end_addr = 0,
    .bss_end_addr = 0,
    .entry_addr = 0x100000,
    .mode_type = LINUX_VIDEO_MODE_LINEAR,
    .width = 1024,
    .height = 768,
    .depth = 32
};


#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY 0xB8000


enum vga_color {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15,
};


static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg);
static inline uint16_t vga_entry(unsigned char uc, uint8_t color);
void terminal_initialize(void);
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y);
void terminal_putchar(char c);
void terminal_writestring(const char* data);

static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_color;
static uint16_t* terminal_buffer;

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
    return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
    return (uint16_t) uc | (uint16_t) color << 8;
}

void terminal_initialize(void) {
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    terminal_buffer = (uint16_t*) VGA_MEMORY;
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = vga_entry(' ', terminal_color);
        }
    }
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = vga_entry(c, color);
}

void terminal_putchar(char c) {
    if (c == '\n') {
        terminal_column = 0;
        if (++terminal_row == VGA_HEIGHT)
            terminal_row = 0;
        return;
    }

    terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
    if (++terminal_column == VGA_WIDTH) {
        terminal_column = 0;
        if (++terminal_row == VGA_HEIGHT)
            terminal_row = 0;
    }
}

void terminal_writestring(const char* data) {
    for (size_t i = 0; data[i] != '\0'; i++)
        terminal_putchar(data[i]);
}

void _start(uint32_t magic, uint32_t boot_info) {
    asm volatile ("cli");
    
    asm volatile ("mov $0x10, %eax");
    asm volatile ("mov %eax, %ds");
    asm volatile ("mov %eax, %es");
    asm volatile ("mov %eax, %fs");
    asm volatile ("mov %eax, %gs");
    asm volatile ("mov %eax, %ss");
    
    asm volatile ("mov $0x7C00, %esp");
    
    if (magic != LINUX_BOOT_MAGIC) {
        while (1) { }
    }
    
    kernel_main(magic, boot_info);
}

void kernel_main(uint32_t magic __attribute__((unused)), uint32_t boot_info) {
    terminal_initialize();
    terminal_writestring("LeOS Kernel Initialized\n");
    terminal_writestring("Setting up system...\n");
    
    struct linux_boot_header* info = (struct linux_boot_header*)boot_info;
    uint32_t* fb = 0;
    
    if (info->flags & LINUX_BOOT_FLAG_VIDEO_MODE) {
        fb = (uint32_t*)(0xFD000000); 
        set_framebuffer(fb);
        draw_desktop();
    } else {
        terminal_writestring("No video mode info from bootloader!\n");
    }
    
    terminal_writestring("System ready!\n");
    while (1) { }
} 