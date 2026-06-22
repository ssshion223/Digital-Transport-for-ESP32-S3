#include <stdio.h>
#include "digital_decoder.h"
#include "audio_player.h"
#include "__I2S.h"

#include <cctype>
#include <cstring>

namespace {
bool ends_with_ext(const char *path, const char *ext) {
    if(path == nullptr || ext == nullptr) {
        return false;
    }
    const size_t path_len = std::strlen(path);
    const size_t ext_len = std::strlen(ext);
    if(path_len < ext_len) {
        return false;
    }
    const char *tail = path + path_len - ext_len;
    for(size_t i = 0; i < ext_len; ++i) {
        unsigned char a = static_cast<unsigned char>(tail[i]);
        unsigned char b = static_cast<unsigned char>(ext[i]);
        if(std::tolower(a) != std::tolower(b)) {
            return false;
        }
    }
    return true;
}
}

DigitalDecoder::DigitalDecoder() {}

DigitalDecoder::~DigitalDecoder() {
    stop();
}

esp_err_t DigitalDecoder::play_wav(const std::string &path) {
    return play_wav(path.c_str());
}

esp_err_t DigitalDecoder::play_wav(const char *path) {
    if(path == nullptr) {
        return ESP_ERR_INVALID_ARG;
    }

    stop_requested_ = false;
    pause_requested_ = false;
    return play_wav_file(path, &stop_requested_, &pause_requested_);
}

esp_err_t DigitalDecoder::play_file(const std::string &path) {
    return play_file(path.c_str());
}

esp_err_t DigitalDecoder::play_file(const char *path) {
    if(path == nullptr) {
        return ESP_ERR_INVALID_ARG;
    }

    stop_requested_ = false;
    pause_requested_ = false;
    if(ends_with_ext(path, ".wav")) {
        return play_wav_file(path, &stop_requested_, &pause_requested_);
    }
    if(ends_with_ext(path, ".flac")) {
        return play_flac_file(path, &stop_requested_, &pause_requested_);
    }

    printf("unsupported audio file: %s\n", path);
    return ESP_ERR_NOT_SUPPORTED;
}

esp_err_t DigitalDecoder::play_wav_async(const char *path) {
    return play_file_async(path);
}

esp_err_t DigitalDecoder::play_wav_async(const std::string &path) {
    return play_wav_async(path.c_str());
}

esp_err_t DigitalDecoder::play_file_async(const char *path) {
    if(audio_command_queue_ == nullptr || path == nullptr) {
        return ESP_ERR_INVALID_STATE;
    }

    AudioCommand cmd = {};
    std::strncpy(cmd.path, path, sizeof(cmd.path) - 1);

    stop_requested_ = true;
    pause_requested_ = false;
    xQueueReset(audio_command_queue_);

    if(xQueueSend(audio_command_queue_, &cmd, 0) != pdTRUE) {
        return ESP_FAIL;
    }

    return ESP_OK;
}

esp_err_t DigitalDecoder::play_file_async(const std::string &path) {
    return play_file_async(path.c_str());
}

void DigitalDecoder::audio_task() {
    AudioCommand cmd;
    while(1) {
        if (xQueueReceive(audio_command_queue_, &cmd, portMAX_DELAY) == pdTRUE) {
            stop_requested_ = false;
            play_file(cmd.path);
        }
    }
}

void DigitalDecoder::audio_task_entry(void *arg) {
    DigitalDecoder *decoder = static_cast<DigitalDecoder *>(arg);
    decoder->audio_task();
}

esp_err_t DigitalDecoder::start() {
    if(audio_task_handle_ != nullptr) {
        return ESP_OK;
    }

    audio_command_queue_ = xQueueCreate(4, sizeof(AudioCommand));
    if (audio_command_queue_ == nullptr) {
        return ESP_ERR_NO_MEM;
    }
    BaseType_t ok = xTaskCreate(audio_task_entry, "audio_task", 16384, this, 6, &audio_task_handle_);
    if (ok != pdPASS) {
        return ESP_FAIL;
    }
    return ESP_OK;
}

void DigitalDecoder::stop() {
    stop_requested_ = true;
    pause_requested_ = false;
    i2s_audio_stop();
}

void DigitalDecoder::pause() {
    pause_requested_ = true;
    i2s_audio_pause();
}

void DigitalDecoder::resume() {
    pause_requested_ = false;
    i2s_audio_resume();
}

bool DigitalDecoder::toggle_pause() {
    if(pause_requested_) {
        resume();
    } else {
        pause();
    }
    return pause_requested_;
}

bool DigitalDecoder::is_paused() const {
    return pause_requested_;
}
