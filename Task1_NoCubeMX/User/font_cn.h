#ifndef __FONT_CN_H
#define __FONT_CN_H

#include <stdint.h>

typedef struct {
    uint32_t code;
    const uint8_t *data;
} CN16;

extern const CN16 cn16_table[];
extern const uint16_t cn16_count;

#endif
