/**
 * @file minino_bsp.c
 * @brief Minino Board Support Package Implementation for ESP32-C6
 */

#include "minino_bsp.h"

#include "esp_log.h"
#include "esp_err.h"
#include "driver/i2c_master.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"

static const char *TAG = "minino_bsp";

/* Static state */
static bool s_i2c_initialized = false;
static bool s_spi_initialized = false;
static bool s_uart_initialized = false;
static i2c_master_bus_handle_t s_i2c_bus = NULL;
static spi_host_device_t s_spi_host = SPI2_HOST;

/* ============================================================================
 * I2C Initialization
 * ============================================================================ */
static esp_err_t minino_i2c_init(void)
{
    if (s_i2c_initialized) {
        return ESP_OK;
    }

    ESP_LOGI(TAG, "Initializing I2C (SDA=%d, SCL=%d)", MININO_I2C_SDA, MININO_I2C_SCL);

    i2c_master_bus_config_t bus_config = {
        .i2c_port = MININO_I2C_NUM,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .intr_priority = 0,
        .trans_queue_depth = 10,
        .flags = {
            .enable_internal_pullup = true,
        },
    };

    esp_err_t ret = i2c_new_master_bus(&bus_config, &s_i2c_bus);
    if (ret == ESP_OK) {
        s_i2c_initialized = true;
        ESP_LOGI(TAG, "I2C initialized successfully");
    } else {
        ESP_LOGE(TAG, "I2C initialization failed: %s", esp_err_to_name(ret));
    }

    return ret;
}

/* ============================================================================
 * SPI Initialization
 * ============================================================================ */
static esp_err_t minino_spi_init(void)
{
    if (s_spi_initialized) {
        return ESP_OK;
    }

    ESP_LOGI(TAG, "Initializing SPI (SCLK=%d, MOSI=%d, MISO=%d)",
             MININO_SPI_SCLK, MININO_SPI_MOSI, MININO_SPI_MISO);

    spi_bus_config_t bus_cfg = {
        .mosi_io_num = MININO_SPI_MOSI,
        .miso_io_num = MININO_SPI_MISO,
        .sclk_io_num = MININO_SPI_SCLK,
        .quadwp_io_num = GPIO_NUM_NC,
        .quadhd_io_num = GPIO_NUM_NC,
        .max_transfer_sz = 4096,
    };

    esp_err_t ret = spi_bus_initialize(MININO_SPI_HOST, &bus_cfg, SPI_DMA_CH_AUTO);
    if (ret == ESP_OK) {
        s_spi_initialized = true;
        ESP_LOGI(TAG, "SPI initialized successfully");
    } else {
        ESP_LOGE(TAG, "SPI initialization failed: %s", esp_err_to_name(ret));
    }

    return ret;
}

/* ============================================================================
 * UART Initialization (GPS)
 * ============================================================================ */
static esp_err_t minino_uart_init(void)
{
    if (s_uart_initialized) {
        return ESP_OK;
    }

    ESP_LOGI(TAG, "Initializing UART (TX=%d, RX=%d, baud=%d)",
             MININO_UART_TX, MININO_UART_RX, MININO_UART_BAUD);

    uart_config_t uart_config = {
        .baud_rate = MININO_UART_BAUD,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };

    esp_err_t ret = uart_driver_install(MININO_UART_NUM, 1024, 0, 0, NULL, 0);
    if (ret == ESP_OK) {
        ret = uart_param_config(MININO_UART_NUM, &uart_config);
        if (ret == ESP_OK) {
            ret = uart_set_pin(MININO_UART_NUM, MININO_UART_TX, MININO_UART_RX,
                              UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
        }
    }

    if (ret == ESP_OK) {
        s_uart_initialized = true;
        ESP_LOGI(TAG, "UART initialized successfully");
    } else {
        ESP_LOGE(TAG, "UART initialization failed: %s", esp_err_to_name(ret));
    }

    return ret;
}

/* ============================================================================
 * ADC Initialization (Battery Voltage)
 * ============================================================================ */
static adc_oneshot_unit_handle_t s_adc1_handle;
static bool s_adc1_initialized = false;

static esp_err_t minino_adc_init(void)
{
    ESP_LOGI(TAG, "Initializing ADC for battery voltage");

    if (s_adc1_initialized) {
        return ESP_OK;
    }

    // Create ADC1 oneshot handle
    adc_oneshot_unit_init_cfg_t init_config = {
        .unit_id = ADC_UNIT_1,
    };
    esp_err_t ret = adc_oneshot_new_unit(&init_config, &s_adc1_handle);
    if (ret == ESP_OK) {
        // Configure ADC channel with attenuation
        adc_oneshot_chan_cfg_t config = {
            .atten = MININO_ADC_ATTEN,
            .bitwidth = MININO_ADC_BIT_WIDTH,
        };
        ret = adc_oneshot_config_channel(s_adc1_handle, MININO_ADC_CHANNEL, &config);
        if (ret == ESP_OK) {
            s_adc1_initialized = true;
            ESP_LOGI(TAG, "ADC1 initialized successfully");
        }
    }

    return ret;
}

/* ============================================================================
 * Public API Implementation
 * ============================================================================ */

esp_err_t minino_bsp_init(uint32_t flags)
{
    esp_err_t ret = ESP_OK;

    ESP_LOGI(TAG, "Initializing Minino BSP (flags=0x%02x)", flags);

    /* Always configure GPIO for buttons */
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << MININO_GPIO_BOOT),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&io_conf);

    /* Configure LED */
    io_conf.pin_bit_mask = (1ULL << MININO_GPIO_LED);
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    gpio_config(&io_conf);
    gpio_set_level(MININO_GPIO_LED, 0);

    if (flags & MININO_BSP_I2C) {
        ret = minino_i2c_init();
        if (ret != ESP_OK) {
            ESP_LOGW(TAG, "I2C init failed, continuing without display/sensors");
        }
    }

    if (flags & MININO_BSP_SPI) {
        ret = minino_spi_init();
        if (ret != ESP_OK) {
            ESP_LOGW(TAG, "SPI init failed, continuing without SD card");
        }
    }

    if (flags & MININO_BSP_UART) {
        ret = minino_uart_init();
        if (ret != ESP_OK) {
            ESP_LOGW(TAG, "UART init failed, continuing without GPS");
        }
    }

    if (flags & MININO_BSP_DISPLAY) {
        if (!s_i2c_initialized) {
            ESP_LOGW(TAG, "Cannot init display without I2C");
        } else {
            /* Display initialization handled by display_task.c */
            ESP_LOGI(TAG, "Display support enabled (OLED I2C)");
        }
    }

    if (flags & MININO_BSP_SD_CARD) {
        if (!s_spi_initialized) {
            ESP_LOGW(TAG, "Cannot init SD card without SPI");
        } else {
            ESP_LOGI(TAG, "SD card support enabled");
        }
    }

    /* Initialize ADC */
    minino_adc_init();

    ESP_LOGI(TAG, "Minino BSP initialization complete");
    return ret;
}

i2c_master_bus_handle_t minino_bsp_get_i2c_bus(void)
{
    return s_i2c_bus;
}

spi_host_device_t minino_bsp_get_spi_bus(void)
{
    return s_spi_host;
}

uart_port_t minino_bsp_get_uart(void)
{
    return MININO_UART_NUM;
}

bool minino_bsp_sd_card_present(void)
{
    if (!s_spi_initialized) {
        return false;
    }

    /* SD card detect would require a specific GPIO */
    /* For now, assume present if SPI is initialized */
    return true;
}

uint16_t minino_bsp_get_battery_voltage(void)
{
    if (!s_adc1_initialized) {
        minino_adc_init();
    }

    // Average multiple readings
    int adc_reading = 0;
    for (int i = 0; i < 16; i++) {
        adc_oneshot_read(s_adc1_handle, MININO_ADC_CHANNEL, &adc_reading);
    }
    adc_reading >>= 4;  // Divide by 16

    // Convert to millivolts (approximate for ESP32-C6)
    // ADC bitwidth is 12-bit, atten 12dB gives 0-3.3V range
    uint32_t voltage = (uint32_t)((adc_reading * 3300) >> 12);

    return (uint16_t)voltage;
}

bool minino_bsp_is_usb_connected(void)
{
    if (MININO_GPIO_VBUS_DETECT != GPIO_NUM_NC) {
        gpio_config_t io_conf = {
            .pin_bit_mask = (1ULL << MININO_GPIO_VBUS_DETECT),
            .mode = GPIO_MODE_INPUT,
            .pull_up_en = GPIO_PULLUP_DISABLE,
            .pull_down_en = GPIO_PULLDOWN_ENABLE,
            .intr_type = GPIO_INTR_DISABLE,
        };
        gpio_config(&io_conf);
        return gpio_get_level(MININO_GPIO_VBUS_DETECT);
    }
    return false;
}
