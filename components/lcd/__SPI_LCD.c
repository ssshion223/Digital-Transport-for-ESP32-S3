#include "__SPI_LCD.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#define LCD_MOSI    GPIO_NUM_47
#define LCD_MISO    -1
#define LCD_SCLK    GPIO_NUM_48
#define LCD_CS      GPIO_NUM_19

spi_device_handle_t lcd_handle;
esp_err_t SPI_Init_LCD() {
    spi_bus_config_t lcd_bus_config = {
        .mosi_io_num = LCD_MOSI,
        .miso_io_num = LCD_MISO,
        .sclk_io_num = LCD_SCLK,
        .max_transfer_sz = 320*480*2,
        .flags = SPICOMMON_BUSFLAG_MASTER,
        .intr_flags = 0,
        .quadhd_io_num = -1,
        .quadwp_io_num = -1,
    };
    esp_err_t err = spi_bus_initialize(SPI2_HOST, &lcd_bus_config, SPI_DMA_CH_AUTO);
    if (err != ESP_OK) {
        return err;
    }
    spi_device_interface_config_t lcd_device_config = {
        .clock_source = SPI_CLK_SRC_DEFAULT,
        .clock_speed_hz = SPI_MASTER_FREQ_80M,
        .mode = 0,
        .queue_size = 7,
        .spics_io_num = LCD_CS,
    };
    err = spi_bus_add_device(SPI2_HOST, &lcd_device_config, &lcd_handle);
    return err;
}

void spi_lcd_transfer_data(const uint8_t *data, const size_t len) {
    spi_transaction_t t = {
        .length = 8 * len,
        .tx_buffer = data,
        .flags = 0,
    };
    spi_device_polling_transmit(lcd_handle, &t);
}
