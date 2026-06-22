#pragma once
#include <stdio.h>
#include "esp_err.h"
#ifdef __cplusplus
extern "C" {
#endif


esp_err_t SPI_Init_LCD();
void spi_lcd_transfer_data(const uint8_t *data, const size_t len);
#ifdef __cplusplus
}
#endif