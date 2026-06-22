#include <stdio.h>
#include "led.h"
#include "driver/gpio.h"
#include "lcd.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sd.h"
#include "lvgl.h"
#include "lv_port.h"
#include "ui.h"
static void sd_file_cb(const char *name, bool is_dir, void *usr) {
    printf("%s %s\n", is_dir?"[DIR]":"[FILE]", name);
}
extern "C" void app_main(void)
{
    vTaskDelay(pdMS_TO_TICKS(200));
    lv_port_init();
    lv_port_start();
    vTaskDelay(pdMS_TO_TICKS(200));

    UI ui;
    ui.init();

    vTaskDelay(pdMS_TO_TICKS(200));
    // SD sd;
    // sd.scan_dir("/sdcard", sd_file_cb, nullptr);
    while(1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
