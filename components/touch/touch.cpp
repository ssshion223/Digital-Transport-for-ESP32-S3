#include <stdio.h>
#include "touch.h"
#include <cstdint>
#include "esp_err.h"
#ifdef __cplusplus
extern "C" {
#endif
#include "__I2C_TOUCH.h"
#ifdef __cplusplus
}
#endif
#define FT6336_REG_ID_STATUS        0x02
#define FT6336_REG_P1_XH            0x03

TOUCH::TOUCH() {
    esp_err_t err = i2c_init_touch();
    if (err == ESP_OK) {
        printf("touch init successfully\r\n");
    } else {
        printf("touch init failed\r\n");
    }
}

TOUCH::~TOUCH() {}

bool TOUCH::read(uint16_t *x, uint16_t *y) {
    uint8_t points = 0;
    if (i2c_touch_read(FT6336_REG_ID_STATUS, &points, 1) != ESP_OK) {
        return false;
    }
    points &= 0x0F;
    if (points == 0) {
        return false;
    }
    uint8_t buf[4] = {0};
    if (i2c_touch_read(FT6336_REG_P1_XH, buf, sizeof(buf)) != ESP_OK) {
        return false;
    }
    *x = ((buf[0] & 0x0F) << 8) | buf[1];
    *y = ((buf[2] & 0x0F) << 8) | buf[3];
    return true;
}