#include "__I2C_TOUCH.h"
#include "driver/gpio.h"
#include "driver/i2c_master.h"
#include "driver/i2c_types.h"

#define SDA_IO      GPIO_NUM_40
#define RST_IO      GPIO_NUM_39
#define SCL_IO      GPIO_NUM_38
#define TOUCH_I2C_PORT      I2C_NUM_0
#define FT6336_I2C_ADDR     0x38
#define TOUCH_I2C_FREQ_HZ   400000

static i2c_master_bus_handle_t s_touch_i2c_bus = NULL;
static i2c_master_dev_handle_t s_touch_i2c_dev = NULL;

esp_err_t i2c_init_touch(void) {
    if (s_touch_i2c_dev != NULL) {
        return ESP_OK;
    }

    i2c_master_bus_config_t bus_config = {
        .i2c_port = TOUCH_I2C_PORT,
        .sda_io_num = SDA_IO,
        .scl_io_num = SCL_IO,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .intr_priority = 0,
        .trans_queue_depth = 0,
        .flags = {
            .enable_internal_pullup = true,
        },
    };

    esp_err_t err = i2c_new_master_bus(&bus_config, &s_touch_i2c_bus);
    if (err != ESP_OK) {
        return err;
    }

    i2c_device_config_t dev_config = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = FT6336_I2C_ADDR,
        .scl_speed_hz = TOUCH_I2C_FREQ_HZ,
        .scl_wait_us = 0,
    };

    err = i2c_master_bus_add_device(s_touch_i2c_bus, &dev_config, &s_touch_i2c_dev);
    if (err != ESP_OK) {
        i2c_del_master_bus(s_touch_i2c_bus);
        s_touch_i2c_bus = NULL;
        s_touch_i2c_dev = NULL;
    }

    return err;
}

static i2c_master_dev_handle_t i2c_touch_get_device(void) {
    return s_touch_i2c_dev;
}

esp_err_t i2c_touch_read(uint8_t reg, uint8_t *data, size_t len) {
    if (s_touch_i2c_dev == NULL) {
        esp_err_t err = i2c_init_touch();
        if (err != ESP_OK) {
            return err;
        }
    }

    return i2c_master_transmit_receive(s_touch_i2c_dev, &reg, 1, data, len, 100);
}
esp_err_t i2c_touch_write(uint8_t reg, uint8_t value) {
    if (s_touch_i2c_dev == NULL) {
        esp_err_t err = i2c_init_touch();
        if (err != ESP_OK) {
            return err;
        }
    }
    uint8_t buffer[2] = {reg, value};
    return i2c_master_transmit(s_touch_i2c_dev, buffer, sizeof(buffer), 100);
}

