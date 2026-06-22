idf_component_register(
    SRCS "lcd.cpp" "__SPI_LCD.c" "lcd_port.cpp"
    INCLUDE_DIRS "include"
    PRIV_REQUIRES esp_driver_spi esp_hal_gpspi esp_driver_gpio lvgl
    )
