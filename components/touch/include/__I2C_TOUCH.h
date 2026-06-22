#pragma once

#include "esp_err.h"
#include <stddef.h>
#include <stdint.h>
// #include "driver/i2c_master.h"

#ifdef __cplusplus
extern "C" {
#endif

esp_err_t i2c_init_touch(void);
esp_err_t i2c_touch_read(uint8_t reg, uint8_t *data, size_t len);
esp_err_t i2c_touch_write(uint8_t reg, uint8_t value);


#ifdef __cplusplus
}
#endif
