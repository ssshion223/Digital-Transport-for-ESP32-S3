#include "__I2S.h"

#include "driver/i2s_std.h"
#include "driver/gpio.h"
#include "esp_check.h"
#include "freertos/FreeRTOS.h"
#define I2S_BCLK_IO GPIO_NUM_16
#define I2S_WS_IO   GPIO_NUM_3
#define I2S_DOUT_IO GPIO_NUM_4

static const char *TAG = "audio_i2s";
static i2s_chan_handle_t s_tx_chan = NULL;
static bool s_tx_enabled = false;

static i2s_data_bit_width_t bits_to_i2s_width(uint8_t bits_per_sample)
{
    switch(bits_per_sample) {
        case 16:
            return I2S_DATA_BIT_WIDTH_16BIT;
        case 24:
            return I2S_DATA_BIT_WIDTH_24BIT;
        case 32:
            return I2S_DATA_BIT_WIDTH_32BIT;
        default:
            return I2S_DATA_BIT_WIDTH_16BIT;
    }
}

static i2s_slot_mode_t channels_to_slot_mode(uint8_t channel_count)
{
    return channel_count == 1 ? I2S_SLOT_MODE_MONO : I2S_SLOT_MODE_STEREO;
}

esp_err_t i2s_audio_init(uint32_t sample_rate, uint8_t bits_per_sample, uint8_t channel_count)
{
    if(s_tx_chan != NULL) {
        return i2s_audio_set_format(sample_rate, bits_per_sample, channel_count);
    }

    i2s_chan_config_t chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_AUTO, I2S_ROLE_MASTER);
    chan_cfg.dma_desc_num = 8;
    chan_cfg.dma_frame_num = 512;

    ESP_RETURN_ON_ERROR(i2s_new_channel(&chan_cfg, &s_tx_chan, NULL), TAG, "create tx channel failed");

    i2s_std_config_t std_cfg = {
        .clk_cfg = I2S_STD_CLK_DEFAULT_CONFIG(sample_rate),
        .slot_cfg = I2S_STD_PHILIPS_SLOT_DEFAULT_CONFIG(bits_to_i2s_width(bits_per_sample),
                                                        channels_to_slot_mode(channel_count)),
        .gpio_cfg = {
            .mclk = I2S_GPIO_UNUSED,
            .bclk = I2S_BCLK_IO,
            .ws = I2S_WS_IO,
            .dout = I2S_DOUT_IO,
            .din = I2S_GPIO_UNUSED,
            .invert_flags = {
                .mclk_inv = false,
                .bclk_inv = false,
                .ws_inv = false,
            },
        },
    };

    esp_err_t err = i2s_channel_init_std_mode(s_tx_chan, &std_cfg);
    if(err != ESP_OK) {
        i2s_del_channel(s_tx_chan);
        s_tx_chan = NULL;
        return err;
    }

    err = i2s_channel_enable(s_tx_chan);
    if(err == ESP_OK) {
        s_tx_enabled = true;
    }
    return err;
}

esp_err_t i2s_audio_set_format(uint32_t sample_rate, uint8_t bits_per_sample, uint8_t channel_count)
{
    ESP_RETURN_ON_FALSE(s_tx_chan != NULL, ESP_ERR_INVALID_STATE, TAG, "i2s is not initialized");

    if(s_tx_enabled) {
        ESP_RETURN_ON_ERROR(i2s_channel_disable(s_tx_chan), TAG, "disable tx channel failed");
        s_tx_enabled = false;
    }

    i2s_std_clk_config_t clk_cfg = I2S_STD_CLK_DEFAULT_CONFIG(sample_rate);
    ESP_RETURN_ON_ERROR(i2s_channel_reconfig_std_clock(s_tx_chan, &clk_cfg), TAG, "reconfig clock failed");

    i2s_std_slot_config_t slot_cfg = I2S_STD_PHILIPS_SLOT_DEFAULT_CONFIG(bits_to_i2s_width(bits_per_sample),
                                                                         channels_to_slot_mode(channel_count));
    ESP_RETURN_ON_ERROR(i2s_channel_reconfig_std_slot(s_tx_chan, &slot_cfg), TAG, "reconfig slot failed");

    esp_err_t err = i2s_channel_enable(s_tx_chan);
    if(err == ESP_OK) {
        s_tx_enabled = true;
    }
    return err;
}

esp_err_t i2s_audio_write(const void *data, size_t size, size_t *bytes_written)
{
    ESP_RETURN_ON_FALSE(s_tx_chan != NULL, ESP_ERR_INVALID_STATE, TAG, "i2s is not initialized");
    if(!s_tx_enabled) {
        ESP_RETURN_ON_ERROR(i2s_channel_enable(s_tx_chan), TAG, "enable tx channel failed");
        s_tx_enabled = true;
    }
    return i2s_channel_write(s_tx_chan, data, size, bytes_written, portMAX_DELAY);
}

esp_err_t i2s_audio_pause(void)
{
    if(s_tx_chan == NULL || !s_tx_enabled) {
        return ESP_OK;
    }
    esp_err_t err = i2s_channel_disable(s_tx_chan);
    if(err == ESP_OK) {
        s_tx_enabled = false;
    }
    return err;
}

esp_err_t i2s_audio_resume(void)
{
    if(s_tx_chan == NULL || s_tx_enabled) {
        return ESP_OK;
    }
    esp_err_t err = i2s_channel_enable(s_tx_chan);
    if(err == ESP_OK) {
        s_tx_enabled = true;
    }
    return err;
}

void i2s_audio_stop(void)
{
    if(s_tx_chan != NULL && s_tx_enabled) {
        i2s_channel_disable(s_tx_chan);
        s_tx_enabled = false;
    }
}

void i2s_audio_deinit(void)
{
    if(s_tx_chan != NULL) {
        if(s_tx_enabled) {
            i2s_channel_disable(s_tx_chan);
            s_tx_enabled = false;
        }
        i2s_del_channel(s_tx_chan);
        s_tx_chan = NULL;
    }
}
