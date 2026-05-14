/**
 * @file minino_bsp.h
 * @brief Minino Board Support Package for ESP32-C6
 * 
 * Minino Hardware: https://github.com/ElectronicCats/Minino
 * MCU: ESP32-C6 (RISC-V, WiFi 6, BLE 5.3, Thread, Zigbee)
 * 
 * Key Features:
 * - ESP32-C6 SoC (RISC-V @ 160MHz)
 * - WiFi 6 (802.11ax) + BLE 5.3
 * - OLED Display (0.91" I2C)
 * - SD Card (SPI)
 * - GPS (UART)
 * - Buttons: BOOT, RESET
 * - LEDs: Status LED
 */

#pragma once

#include "driver/gpio.h"
#include "driver/i2c_master.h"
#include "driver/spi_master.h"
#include "driver/uart.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * Board Identification
 * ============================================================================ */
#define MININO_BOARD_NAME           "Electronic Cats Minino"
#define MININO_MCU_TARGET           "ESP32-C6"
#define MININO_MCU_FAMILY           "esp32c6"

/* ============================================================================
 * GPIO Pin Mapping (ESP32-C6 Minino)
 * Based on: https://github.com/ElectronicCats/Minino
 * ============================================================================ */

/* ---- WiFi Antenna (Internal) ---- */
/* ESP32-C6 has internal WiFi antenna */

/* ---- LEDs ---- */
#define MININO_GPIO_LED             GPIO_NUM_8      /* Status LED */
#define MININO_GPIO_LED_MAX         1

/* ---- Buttons ---- */
#define MININO_GPIO_BOOT            GPIO_NUM_9      /* BOOT button */
#define MININO_GPIO_RESET           GPIO_NUM_1      /* RESET button */

/* ---- I2C Bus (OLED Display, Sensors) ---- */
#define MININO_I2C_NUM              I2C_NUM_0
#define MININO_I2C_SDA              GPIO_NUM_1      /* I2C SDA */
#define MININO_I2C_SCL              GPIO_NUM_2      /* I2C SCL */
#define MININO_I2C_FREQ_HZ          400000          /* 400kHz */

/* ---- SPI Bus (SD Card, External Flash) ---- */
#define MININO_SPI_HOST             SPI2_HOST
#define MININO_SPI_SCLK             GPIO_NUM_4      /* SPI SCLK */
#define MININO_SPI_MOSI             GPIO_NUM_5      /* SPI MOSI */
#define MININO_SPI_MISO             GPIO_NUM_6      /* SPI MISO */
#define MININO_SPI_CS_SD            GPIO_NUM_7      /* SD Card CS */

/* ---- UART (GPS Module) ---- */
#define MININO_UART_NUM             UART_NUM_1
#define MININO_UART_TX              GPIO_NUM_3      /* UART TX to GPS */
#define MININO_UART_RX              GPIO_NUM_4      /* UART RX from GPS */
#define MININO_UART_BAUD            9600            /* GPS baud rate */

/* ---- Power Management ---- */
#define MININO_GPIO_VBUS_DETECT     GPIO_NUM_10     /* USB VBUS detect */
#define MININO_GPIO_BAT_VOLTAGE     GPIO_NUM_5      /* Battery voltage ADC */

/* ============================================================================
 * Display (OLED 0.91" I2C)
 * ============================================================================ */
#define MININO_DISPLAY_I2C_ADDR     0x3C            /* OLED I2C address */
#define MININO_DISPLAY_WIDTH        128             /* OLED width */
#define MININO_DISPLAY_HEIGHT       64              /* OLED height */
#define MININO_DISPLAY_RST          GPIO_NUM_NC     /* No hardware reset pin */

/* ============================================================================
 * SD Card Configuration
 * ============================================================================ */
#define MININO_SD_SPI_HOST          SPI2_HOST
#define MININO_SD_SPI_SCLK          MININO_SPI_SCLK
#define MININO_SD_SPI_MOSI          MININO_SPI_MOSI
#define MININO_SD_SPI_MISO          MININO_SPI_MISO
#define MININO_SD_SPI_CS            MININO_SPI_CS_SD
#define MININO_SD_MAX_FREQ_MHZ      20              /* SD card SPI frequency */

/* ============================================================================
* ADC Configuration
* ============================================================================ */
/* Note: ESP32-C6 ADC1 Channel 4 = GPIO5 */
#define MININO_ADC_UNIT             ADC_UNIT_1
#define MININO_ADC_CHANNEL          ADC_CHANNEL_4   /* ADC1_CH4 = GPIO5 on ESP32-C6 */
#define MININO_ADC_ATTEN            ADC_ATTEN_DB_12 /* 0-3.3V range (12dB) */
#define MININO_ADC_BIT_WIDTH        SOC_ADC_DIGI_MAX_BITWIDTH

/* ============================================================================
 * Deep Sleep Wakeup Sources
 * ============================================================================ */
#define MININO_WAKEUP_GPIO_MASK     (1ULL << MININO_GPIO_BOOT)
#define MININO_WAKEUP_TOUCHPAD_MASK 0

/* ============================================================================
 * BSP Initialization Flags
 * ============================================================================ */
typedef enum {
    MININO_BSP_I2C          = (1 << 0),     /* Initialize I2C */
    MININO_BSP_SPI          = (1 << 1),     /* Initialize SPI */
    MININO_BSP_UART         = (1 << 2),     /* Initialize UART */
    MININO_BSP_DISPLAY      = (1 << 3),     /* Initialize OLED display */
    MININO_BSP_SD_CARD      = (1 << 4),     /* Initialize SD card */
    MININO_BSP_ALL          = 0xFF          /* Initialize everything */
} minino_bsp_flags_t;

/* ============================================================================
 * Public API
 * ============================================================================ */

/**
 * @brief Initialize Minino board peripherals
 * 
 * @param flags Bitmask of minino_bsp_flags_t
 * @return esp_err_t ESP_OK on success
 */
esp_err_t minino_bsp_init(uint32_t flags);

/**
 * @brief Get I2C master handle
 * @return i2c_master_bus_handle_t I2C bus handle
 */
i2c_master_bus_handle_t minino_bsp_get_i2c_bus(void);

/**
 * @brief Get SPI host device
 * @return spi_host_device_t SPI host device
 */
spi_host_device_t minino_bsp_get_spi_bus(void);

/**
 * @brief Get UART handle
 * @return uart_port_t UART port number
 */
uart_port_t minino_bsp_get_uart(void);

/**
 * @brief Check if SD card is present
 * @return true if SD card detected
 */
bool minino_bsp_sd_card_present(void);

/**
 * @brief Get battery voltage in mV
 * @return Battery voltage in millivolts
 */
uint16_t minino_bsp_get_battery_voltage(void);

/**
 * @brief Check if USB is connected
 * @return true if USB VBUS detected
 */
bool minino_bsp_is_usb_connected(void);

#ifdef __cplusplus
}
#endif
