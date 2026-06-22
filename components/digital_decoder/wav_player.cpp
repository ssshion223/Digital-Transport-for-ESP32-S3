#include "audio_player.h"
#include "__I2S.h"

#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

namespace {

constexpr size_t kPcmBufferSize = 16 * 1024;

struct WavFormat {
    uint16_t audio_format = 0;
    uint16_t channels = 0;
    uint32_t sample_rate = 0;
    uint16_t bits_per_sample = 0;
    uint32_t data_size = 0;
};

bool read_exact(FILE *fp, void *dst, size_t size) {
    return fread(dst, 1, size, fp) == size;
}

uint16_t read_le16(const uint8_t *p) {
    return static_cast<uint16_t>(p[0]) | (static_cast<uint16_t>(p[1]) << 8);
}

uint32_t read_le32(const uint8_t *p) {
    return static_cast<uint32_t>(p[0]) |
           (static_cast<uint32_t>(p[1]) << 8) |
           (static_cast<uint32_t>(p[2]) << 16) |
           (static_cast<uint32_t>(p[3]) << 24);
}

esp_err_t parse_wav_header(FILE *fp, WavFormat *format) {
    uint8_t riff_header[12];
    if(!read_exact(fp, riff_header, sizeof(riff_header))) {
        return ESP_ERR_INVALID_SIZE;
    }

    if(std::memcmp(riff_header, "RIFF", 4) != 0 || std::memcmp(riff_header + 8, "WAVE", 4) != 0) {
        return ESP_ERR_INVALID_RESPONSE;
    }

    bool found_fmt = false;
    bool found_data = false;

    while(!found_data) {
        uint8_t chunk_header[8];
        if(!read_exact(fp, chunk_header, sizeof(chunk_header))) {
            return ESP_ERR_NOT_FOUND;
        }

        uint32_t chunk_size = read_le32(chunk_header + 4);
        long chunk_data_start = ftell(fp);

        if(std::memcmp(chunk_header, "fmt ", 4) == 0) {
            uint8_t fmt[16];
            if(chunk_size < sizeof(fmt) || !read_exact(fp, fmt, sizeof(fmt))) {
                return ESP_ERR_INVALID_SIZE;
            }
            format->audio_format = read_le16(fmt + 0);
            format->channels = read_le16(fmt + 2);
            format->sample_rate = read_le32(fmt + 4);
            format->bits_per_sample = read_le16(fmt + 14);
            found_fmt = true;
        } else if(std::memcmp(chunk_header, "data", 4) == 0) {
            if(!found_fmt) {
                return ESP_ERR_INVALID_STATE;
            }
            format->data_size = chunk_size;
            found_data = true;
            break;
        }

        if(!found_data) {
            long next = chunk_data_start + chunk_size + (chunk_size & 1);
            if(fseek(fp, next, SEEK_SET) != 0) {
                return ESP_FAIL;
            }
        }
    }

    if(format->audio_format != 1) {
        printf("unsupported wav format: %u\n", format->audio_format);
        return ESP_ERR_NOT_SUPPORTED;
    }
    if(format->bits_per_sample != 16) {
        printf("unsupported wav bits: %u\n", format->bits_per_sample);
        return ESP_ERR_NOT_SUPPORTED;
    }
    if(format->channels != 1 && format->channels != 2) {
        printf("unsupported wav channels: %u\n", format->channels);
        return ESP_ERR_NOT_SUPPORTED;
    }

    return ESP_OK;
}

} // namespace

esp_err_t play_wav_file(const char *path, volatile bool *stop_requested, volatile bool *pause_requested) {
    FILE *fp = fopen(path, "rb");
    if(fp == nullptr) {
        printf("open wav failed: %s\n", path);
        return ESP_FAIL;
    }

    WavFormat format;
    esp_err_t err = parse_wav_header(fp, &format);
    if(err != ESP_OK) {
        fclose(fp);
        return err;
    }

    err = i2s_audio_init(format.sample_rate, format.bits_per_sample, format.channels);
    if(err != ESP_OK) {
        fclose(fp);
        printf("i2s init failed: 0x%x\n", static_cast<unsigned>(err));
        return err;
    }

    uint8_t *buffer = static_cast<uint8_t *>(std::malloc(kPcmBufferSize));
    if(buffer == nullptr) {
        fclose(fp);
        printf("alloc wav pcm buffer failed\n");
        return ESP_ERR_NO_MEM;
    }

    uint32_t remain = format.data_size;
    while(remain > 0 && (stop_requested == nullptr || !*stop_requested)) {
        while((pause_requested != nullptr && *pause_requested) &&
              (stop_requested == nullptr || !*stop_requested)) {
            vTaskDelay(pdMS_TO_TICKS(20));
        }
        if(stop_requested != nullptr && *stop_requested) {
            break;
        }

        size_t to_read = std::min<size_t>(kPcmBufferSize, remain);
        size_t read_len = fread(buffer, 1, to_read, fp);
        if(read_len == 0) {
            break;
        }

        size_t written = 0;
        err = i2s_audio_write(buffer, read_len, &written);
        if(err != ESP_OK) {
            printf("i2s write failed: 0x%x\n", static_cast<unsigned>(err));
            break;
        }

        remain -= read_len;
    }

    std::free(buffer);
    fclose(fp);
    return err;
}
