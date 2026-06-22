#include <stdio.h>
#include "lv_port.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "esp_timer.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
static esp_timer_handle_t s_lv_tick_timer;
static bool s_lvgl_task_started = false;
static void lv_tick_cb(void *arg) {
    (void)arg;
    lv_tick_inc(1);
}

static esp_err_t start_lv_tick_timer() {
    const esp_timer_create_args_t tick_timer_args = {
        .callback = lv_tick_cb,
        .arg = nullptr,
        .dispatch_method = ESP_TIMER_TASK,
        .name = "lv_tick",
        .skip_unhandled_events = true
    };
    esp_err_t err = esp_timer_create(&tick_timer_args, &s_lv_tick_timer);
    if (err != ESP_OK) {
        s_lv_tick_timer = nullptr;
        return err;
    }
    err = esp_timer_start_periodic(s_lv_tick_timer, 1000);
    if (err != ESP_OK) {
        esp_timer_delete(s_lv_tick_timer);
        s_lv_tick_timer = nullptr;
    }
    return err;
}
static void lvgl_task(void *arg) {
    (void)arg;
    while (1) {
        lv_timer_handler();
        vTaskDelay(pdMS_TO_TICKS(5));
    }
}
void lv_port_init() {
    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();
    start_lv_tick_timer();
}

void lv_port_start() {
    if (s_lvgl_task_started) {
        return;
    }
    xTaskCreate(lvgl_task, "lvgl", 8192, nullptr, 5, nullptr);
    s_lvgl_task_started = true;
}
