#include "includes/util/log-info.h"
#include <stdarg.h>
const char* result_str[ResultCount] = {
    [Ok]    = "OK",
    [Warn]  = "WARN",
    [Error] = "ERROR",
    [Fatal] = "FATAL",
};

void log_to_terminal(result_t status, const char *from, const char *file, int line, const char *fmt, ...) {
    //build msg
    char message[512];
    char *ptr = message;
    
    //add the fun part, the color
    const char *color = get_status_color(status);
    while (*color) *ptr++ = *color++;
    
    //this i so i dont forget: this is how spacing is introduced:
    *ptr++ = '[';
    *ptr++ = ' ';
    *ptr++ = ' ';
    
    const char *status_str = result_str[status];
    while (*status_str) *ptr++ = *status_str++;
    
    *ptr++ = ' ';
    *ptr++ = ' ';
    *ptr++ = ']';
    //=================================

    // this resets color so the entire terminal isnt one color or the other
    const char *reset = COLOR_RESET;
    while (*reset) *ptr++ = *reset++;
    *ptr++ = ' ';
    
    //fn name and location
    while (*from) *ptr++ = *from++;
    
    *ptr++ = ' ';
    *ptr++ = 'i';
    *ptr++ = 'n';
    *ptr++ = ' ';
    
    //full path/file name
    const char *file_ptr = file;
    while (*file_ptr) *ptr++ = *file_ptr++;

    *ptr++ = ' ';
    *ptr++ = 'a';
    *ptr++ = 't';
    *ptr++ = ' ';
    *ptr++ = 'l';
    *ptr++ = 'i';
    *ptr++ = 'n';
    *ptr++ = 'e';
    *ptr++ = ' ';
    
    //ln number
    char line_str[16];
    itoa(line, line_str, 10);
    const char *line_ptr = line_str;
    while (*line_ptr) *ptr++ = *line_ptr++;
    
    *ptr++ = ':';
    *ptr++ = ' ';
    
    //js the formatted message
    va_list args;
    va_start(args, fmt);
    
    while (*fmt) {
        if (*fmt == '%') {
            fmt++;
            if (*fmt == 'd' || *fmt == 'i') {
                int val = va_arg(args, int);
                char num[16];
                itoa(val, num, 10);
                const char *num_ptr = num;
                while (*num_ptr) *ptr++ = *num_ptr++;
            } else if (*fmt == 'u') {
                unsigned int val = va_arg(args, unsigned int);
                char num[16];
                itoa((int)val, num, 10);
                const char *num_ptr = num;
                while (*num_ptr) *ptr++ = *num_ptr++;
            } else if (*fmt == 's') {
                char *str = va_arg(args, char*);
                while (*str) *ptr++ = *str++;
            } else if (*fmt == 'p') {
                void *p = va_arg(args, void*);
                *ptr++ = '0';
                *ptr++ = 'x';
                char hex[20];
                itoa((int)(uintptr_t)p, hex, 16);
                const char *hex_ptr = hex;
                while (*hex_ptr) *ptr++ = *hex_ptr++;
            } else if (*fmt == 'x' || *fmt == 'X') {
                unsigned int val = va_arg(args, unsigned int);
                char hex[16];
                itoa((int)val, hex, 16);
                const char *hex_ptr = hex;
                while (*hex_ptr) *ptr++ = *hex_ptr++;
            } else if (*fmt == '%') {
                *ptr++ = '%';
            } else {
                *ptr++ = '%';
                if (*fmt) *ptr++ = *fmt;
            }
        } else {
            *ptr++ = *fmt;
        }
        if (*fmt) fmt++;
    }
    va_end(args);
    
    *ptr = '\0';
    writestr(&fb_term, message, ptr - message);
}