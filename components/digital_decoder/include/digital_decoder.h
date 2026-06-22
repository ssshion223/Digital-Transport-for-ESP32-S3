#pragma once

#include "esp_err.h"
#include <string>
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
class DigitalDecoder {
public:
    DigitalDecoder();
    ~DigitalDecoder();
    esp_err_t play_wav(const char *path);
    esp_err_t play_wav(const std::string &path);
    esp_err_t play_file(const char *path);
    esp_err_t play_file(const std::string &path);
    esp_err_t start();
    esp_err_t play_wav_async(const char *path);
    esp_err_t play_wav_async(const std::string &path);
    esp_err_t play_file_async(const char *path);
    esp_err_t play_file_async(const std::string &path);
    void pause();
    void resume();
    bool toggle_pause();
    bool is_paused() const;
    void stop();
private:
    volatile bool stop_requested_ = false;
    volatile bool pause_requested_ = false;
    struct AudioCommand {
        char path[256];
    };
    static void audio_task_entry(void *arg);
    void audio_task();
    QueueHandle_t audio_command_queue_ = nullptr;
    TaskHandle_t audio_task_handle_ = nullptr;
};
