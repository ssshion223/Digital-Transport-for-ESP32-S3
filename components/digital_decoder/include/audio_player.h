#pragma once

#include "esp_err.h"

esp_err_t play_wav_file(const char *path, volatile bool *stop_requested, volatile bool *pause_requested);
esp_err_t play_flac_file(const char *path, volatile bool *stop_requested, volatile bool *pause_requested);
