// Author: Jerry Jhird
// Project: CuoreOS
// License: MPLv2.0, relicensed under GPLv2 for ancoreOS

#ifndef KEYBOARD_H
#define KEYBOARD_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>   

extern const char scta_uk[128];
extern const char scta_uk_shift[128];

// funcs
char getc(void);
bool ps2_dev_exists(uint8_t port);

#ifdef __cplusplus
}
#endif

#endif 