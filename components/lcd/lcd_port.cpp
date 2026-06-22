#include "lcd_port.h"
#include "lcd.h"

static LCD &get_lcd() {
    static LCD lcd;
    return lcd;
}
void lcd_port_init() {
    (void)get_lcd();
}

void lcd_port_draw_bitmap(uint16_t x1, uint16_t y1, 
    uint16_t x2, uint16_t y2, const uint8_t *data) {
    get_lcd().draw_bitmap(x1, y1, x2, y2, data);
}