#include "string.h"

void* memset(void* ptr, int value, size_t num) {
    unsigned char* p = ptr;
    while (num--) {
        *p++ = (unsigned char)value;
    }
    return ptr;
}

void* memcpy(void* destination, const void* source, size_t num) {
    unsigned char* dst = destination;
    const unsigned char* src = source;
    while (num--) {
        *dst++ = *src++;
    }
    return destination;
}

size_t strlen(const char* str) {
    size_t len = 0;
    while (str[len]) {
        len++;
    }
    return len;
}

char* strcpy(char* destination, const char* source) {
    char* dst = destination;
    while ((*dst++ = *source++));
    return destination;
}

int strcmp(const char* str1, const char* str2) {
    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    return *(unsigned char*)str1 - *(unsigned char*)str2;
}

char* strncpy(char* destination, const char* source, size_t num) {
    char* dst = destination;
    while (num && (*dst++ = *source++)) {
        num--;
    }
    while (num--) {
        *dst++ = '\0';
    }
    return destination;
} 