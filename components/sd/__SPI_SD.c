#include "__SPI_SD.h"
#include "esp_vfs_fat.h"
#include "driver/gpio.h"
#include "esp_err.h"
#include "driver/spi_master.h"
#include <stdio.h>
#define SD_CS_IO        GPIO_NUM_10
#define SD_SCK_IO       GPIO_NUM_11
#define SD_MOSI_IO      GPIO_NUM_12
#define SD_MISO_IO      GPIO_NUM_13
sdmmc_card_t *sdcard;
esp_err_t spi_sd_init() {   
    spi_bus_config_t sd_bus_config = {
        .mosi_io_num = SD_MOSI_IO,
        .miso_io_num = SD_MISO_IO,
        .sclk_io_num = SD_SCK_IO,
        .flags = SPICOMMON_BUSFLAG_MASTER,
        .intr_flags = 0,
        .quadhd_io_num = -1,
        .quadwp_io_num = -1,
    };
    esp_err_t err = spi_bus_initialize(SPI3_HOST, &sd_bus_config, SPI_DMA_CH_AUTO);
    if (err != ESP_OK) {
        return err;
    }
    
    printf("spi host3 initailization successful\r\n");
    sdmmc_host_t host = SDSPI_HOST_DEFAULT();
    host.slot = SPI3_HOST;
    host.max_freq_khz = SDMMC_FREQ_HIGHSPEED;
    // sdspi_host_init_device();
    sdspi_device_config_t slot = SDSPI_DEVICE_CONFIG_DEFAULT();
    slot.host_id = SPI3_HOST;
    slot.gpio_cs = SD_CS_IO;
    esp_vfs_fat_mount_config_t mount = {
        .format_if_mount_failed = false,
        .disk_status_check_enable = false,
        .max_files = 5,
        .allocation_unit_size = 16*1024,
    };
    err = esp_vfs_fat_sdspi_mount(
        "/sdcard",
        &host,
        &slot,
        &mount,
        &sdcard);
    return err;
}