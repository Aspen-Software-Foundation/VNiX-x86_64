/*
            The AMPOS Operating System is
            copyright under the Aspen Software Foundation (And the file's corresponding developers)
            
            This project is licensed under the GNU Public License v2;
            For more information, visit "https://www.gnu.org/licenses/gpl-2.0.en.html"
            OR see to the "LICENSE" file.

*/

#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>
#include <stddef.h>


// Utility functions
void *memset(void *ptr, int value, size_t num);
void *memcpy(void *dest, const void *src, size_t n);
int8_t memcmp(const char *str1, const char *str2, size_t n);
void *memmove(void *dst, const void *src, size_t n);

void *memset_pattern(void *ptr, const void *pattern, size_t pattern_size, size_t num);
int8_t memcmp_const(const void *ptr1, const uint8_t val, size_t n);
void *itoa(int32_t value, char *str, uint32_t base);
void* malloc(size_t size);
void free(void* ptr, size_t size);

#endif // MEMORY_H