#pragma once

#include "esp_err.h"
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

esp_err_t i2s_audio_init(uint32_t sample_rate, uint8_t bits_per_sample, uint8_t channel_count);
esp_err_t i2s_audio_set_format(uint32_t sample_rate, uint8_t bits_per_sample, uint8_t channel_count);
esp_err_t i2s_audio_write(const void *data, size_t size, size_t *bytes_written);
esp_err_t i2s_audio_pause(void);
esp_err_t i2s_audio_resume(void);
void i2s_audio_stop(void);
void i2s_audio_deinit(void);

#ifdef __cplusplus
}
#endif
