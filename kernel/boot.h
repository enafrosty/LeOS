#ifndef BOOT_H
#define BOOT_H

#include <stdint.h>


#define LINUX_BOOT_MAGIC 0x1BADB002


struct linux_boot_header {
    uint32_t magic;          /* Magic number */
    uint32_t flags;          /* Boot flags */
    uint32_t checksum;       /* Checksum */
    uint32_t header_addr;    /* Header address */
    uint32_t load_addr;      /* Load address */
    uint32_t load_end_addr;  /* Load end address */
    uint32_t bss_end_addr;   /* BSS end address */
    uint32_t entry_addr;     /* Entry point address */
    uint32_t mode_type;      /* Video mode type */
    uint32_t width;          /* Video mode width */
    uint32_t height;         /* Video mode height */
    uint32_t depth;          /* Video mode depth */
};


#define LINUX_BOOT_FLAG_MEMORY_INFO 0x00000001
#define LINUX_BOOT_FLAG_VIDEO_MODE  0x00000002
#define LINUX_BOOT_FLAG_LOAD_HIGH   0x00000004


#define LINUX_VIDEO_MODE_LINEAR 0x00000001

#endif 