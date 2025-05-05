#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "string.h"

#define MAX_FILENAME_LENGTH 32
#define MAX_PATH_LENGTH 256
#define SECTOR_SIZE 512
#define MAX_FILES_PER_DIR 128

typedef enum {
    FILE_TYPE_REGULAR,
    FILE_TYPE_DIRECTORY,
    FILE_TYPE_SYMLINK
} file_type_t;

typedef struct {
    char name[MAX_FILENAME_LENGTH];
    file_type_t type;
    uint32_t size;
    uint32_t first_sector;
    uint32_t parent_dir;
    uint32_t creation_time;
    uint32_t modification_time;
} file_entry_t;

typedef struct {
    file_entry_t entries[MAX_FILES_PER_DIR];
    uint32_t entry_count;
} directory_t;

static directory_t root_dir;
static directory_t current_dir;
static uint32_t* disk_buffer = (uint32_t*)0x1000000;

void fs_init(void) {
    memset(&root_dir, 0, sizeof(directory_t));
    strcpy(root_dir.entries[0].name, ".");
    root_dir.entries[0].type = FILE_TYPE_DIRECTORY;
    root_dir.entries[0].parent_dir = 0;
    root_dir.entry_count = 1;
    current_dir = root_dir;
}

bool read_sector(uint32_t sector, void* buffer) {
    memcpy(buffer, disk_buffer + (sector * SECTOR_SIZE), SECTOR_SIZE);
    return true;
}

bool write_sector(uint32_t sector, const void* buffer) {
    memcpy(disk_buffer + (sector * SECTOR_SIZE), buffer, SECTOR_SIZE);
    return true;
}

bool fs_create_file(const char* name, file_type_t type) {
    if (current_dir.entry_count >= MAX_FILES_PER_DIR) {
        return false;
    }
    
    for (uint32_t i = 0; i < current_dir.entry_count; i++) {
        if (strcmp(current_dir.entries[i].name, name) == 0) {
            return false;
        }
    }
    
    file_entry_t* new_entry = &current_dir.entries[current_dir.entry_count];
    strncpy(new_entry->name, name, MAX_FILENAME_LENGTH - 1);
    new_entry->type = type;
    new_entry->size = 0;
    new_entry->first_sector = 0;
    new_entry->parent_dir = 0;
    new_entry->creation_time = 0;
    new_entry->modification_time = 0;
    
    current_dir.entry_count++;
    return true;
}

void fs_list_directory(void) {
    for (uint32_t i = 0; i < current_dir.entry_count; i++) {
        const file_entry_t* entry = &current_dir.entries[i];
        (void)entry;
    }
}

bool fs_change_directory(const char* path) {
    (void)path;
    return false;
}

bool fs_read_file(const char* name, void* buffer, uint32_t size, uint32_t offset) {
    file_entry_t* file = NULL;
    for (uint32_t i = 0; i < current_dir.entry_count; i++) {
        if (strcmp(current_dir.entries[i].name, name) == 0) {
            file = &current_dir.entries[i];
            break;
        }
    }
    
    if (!file || file->type != FILE_TYPE_REGULAR) {
        return false;
    }
    
    uint32_t sector = file->first_sector + (offset / SECTOR_SIZE);
    uint32_t bytes_read = 0;
    uint32_t start_offset = offset % SECTOR_SIZE;
    
    while (bytes_read < size && sector != 0) {
        uint8_t sector_buffer[SECTOR_SIZE];
        read_sector(sector, sector_buffer);
        
        uint32_t bytes_to_copy = SECTOR_SIZE - start_offset;
        if (bytes_to_copy > (size - bytes_read)) {
            bytes_to_copy = size - bytes_read;
        }
        
        memcpy(buffer + bytes_read, sector_buffer + start_offset, bytes_to_copy);
        bytes_read += bytes_to_copy;
        sector++;
        start_offset = 0;
    }
    
    return true;
}

bool fs_write_file(const char* name, const void* buffer, uint32_t size) {
    file_entry_t* file = NULL;
    for (uint32_t i = 0; i < current_dir.entry_count; i++) {
        if (strcmp(current_dir.entries[i].name, name) == 0) {
            file = &current_dir.entries[i];
            break;
        }
    }
    
    if (!file || file->type != FILE_TYPE_REGULAR) {
        return false;
    }
    
    uint32_t sector = file->first_sector;
    uint32_t bytes_written = 0;
    
    while (bytes_written < size) {
        write_sector(sector, buffer + bytes_written);
        bytes_written += SECTOR_SIZE;
    }
    
    file->size = size;
    file->modification_time = 0;
    return true;
}