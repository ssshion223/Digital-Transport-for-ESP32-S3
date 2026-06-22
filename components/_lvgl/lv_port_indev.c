#include "lv_port_indev.h"
#include "touch_port.h"

static void touchpad_read(lv_indev_t *indev, lv_indev_data_t *data);

static lv_indev_t *indev_touchpad;

void lv_port_indev_init(void)
{
    touch_port_init();

    indev_touchpad = lv_indev_create();
    lv_indev_set_type(indev_touchpad, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev_touchpad, touchpad_read);
}

static void touchpad_read(lv_indev_t *indev, lv_indev_data_t *data)
{
    (void)indev;

    static int32_t last_x = 0;
    static int32_t last_y = 0;

    uint16_t x = 0;
    uint16_t y = 0;

    if (touch_port_read(&x, &y)) {
        last_x = x;
        last_y = y;
        data->state = LV_INDEV_STATE_PRESSED;
    } else {
        data->state = LV_INDEV_STATE_RELEASED;
    } 
    data->point.x = last_x;
    data->point.y = last_y;
}