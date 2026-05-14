/**
 * @file main.h
 * @brief Main header for Minino ESP32-C6 CSI Node
 */

#pragma once

#include "esp_idf_version.h"

/* ESP-IDF version check */
#if ESP_IDF_VERSION_MAJOR < 6
#warning "This firmware is designed for ESP-IDF v6.x"
#endif

/* Target check */
#ifdef CONFIG_IDF_TARGET
#if CONFIG_IDF_TARGET_ESP32C6
#define MININO_TARGET "esp32c6"
#elif CONFIG_IDF_TARGET_ESP32S3
#define MININO_TARGET "esp32s3"
#warning "Building for ESP32-S3, not ESP32-C6"
#else
#define MININO_TARGET CONFIG_IDF_TARGET
#endif
#endif
