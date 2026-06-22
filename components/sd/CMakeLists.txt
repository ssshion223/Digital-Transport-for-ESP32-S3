idf_component_register(
    SRCS "sd.cpp" "__SPI_SD.c"
    INCLUDE_DIRS "include"
    PRIV_REQUIRES driver fatfs esp_driver_gpio esp_driver_spi
)
