idf_component_register(
    SRCS "digital_decoder.cpp" "wav_player.cpp" "flac_player.cpp" "__I2S.c"
    INCLUDE_DIRS "include" "third_party"
    PRIV_REQUIRES driver esp_driver_i2s esp_driver_gpio
    )
