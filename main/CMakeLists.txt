idf_component_register(
    SRCS "main.cpp"
    INCLUDE_DIRS "."
    REQUIRES driver esp_driver_gpio
    PRIV_REQUIRES led lcd sd lvgl _lvgl ui digital_decoder
    )
