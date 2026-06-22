#include "touch_port.h"
#include "touch.h"

static TOUCH &get_touch() {
    static TOUCH touch;
    return touch;
}
void touch_port_init() {
    (void)get_touch();
}
bool touch_port_read(uint16_t *x, uint16_t *y) {
    return get_touch().read(x, y);
}
