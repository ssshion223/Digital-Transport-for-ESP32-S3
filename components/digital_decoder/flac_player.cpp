#include "audio_player.h"
#include "__I2S.h"

#define DR_FLAC_NO_OGG
#define DR_FLAC_NO_WCHAR
#define DR_FLAC_IMPLEMENTATION
#include "dr_flac.h"

#include <cstdlib>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

namespace {
constexpr drflac_uint64 kFramesPerRead = 1024;
}

esp_err_t play_flac_file(const char *path, volatile bool *stop_requested, volatile bool *pause_requested) {
    drflac *flac = drflac_open_file(path, nullptr);
    if(flac == nullptr) {
        printf("open flac failed: %s\n", path);
        return ESP_FAIL;
    }

    if(flac->channels != 1 && flac->channels != 2) {
        printf("unsupported flac channels: %u\n", flac->channels);
        drflac_close(flac);
        return ESP_ERR_NOT_SUPPORTED;
    }

    esp_err_t err = i2s_audio_init(flac->sampleRate, 16, flac->channels);
    if(err != ESP_OK) {
        printf("i2s init failed: 0x%x\n", static_cast<unsigned>(err));
        drflac_close(flac);
        return err;
    }

    size_t sample_count = static_cast<size_t>(kFramesPerRead) * flac->channels;
    drflac_int16 *buffer = static_cast<drflac_int16 *>(std::malloc(sample_count * sizeof(drflac_int16)));
    if(buffer == nullptr) {
        printf("alloc flac pcm buffer failed\n");
        drflac_close(flac);
        return ESP_ERR_NO_MEM;
    }

    while(stop_requested == nullptr || !*stop_requested) {
        while((pause_requested != nullptr && *pause_requested) &&
              (stop_requested == nullptr || !*stop_requested)) {
            vTaskDelay(pdMS_TO_TICKS(20));
        }
        if(stop_requested != nullptr && *stop_requested) {
            break;
        }

        drflac_uint64 frames_read = drflac_read_pcm_frames_s16(flac, kFramesPerRead, buffer);
        if(frames_read == 0) {
            break;
        }

        size_t bytes_to_write = static_cast<size_t>(frames_read) * flac->channels * sizeof(drflac_int16);
        size_t written = 0;
        err = i2s_audio_write(buffer, bytes_to_write, &written);
        if(err != ESP_OK) {
            printf("i2s write failed: 0x%x\n", static_cast<unsigned>(err));
            break;
        }
    }

    std::free(buffer);
    drflac_close(flac);
    return err;
}
