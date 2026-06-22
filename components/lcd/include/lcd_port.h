#pragma once
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
void lcd_port_init();
void lcd_port_draw_bitmap(uint16_t x1, uint16_t y1, 
    uint16_t x2, uint16_t y2, const uint8_t *data);

#ifdef __cplusplus
}
#endif