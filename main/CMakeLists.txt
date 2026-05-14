set(SRCS
  "main.c" "csi_collector.c" "stream_sender.c" "nvs_config.c"
  "edge_processing.c" "ota_update.c" "power_mgmt.c"
  "wasm_runtime.c" "wasm_upload.c" "rvf_parser.c"
  "mmwave_sensor.c"
  "swarm_bridge.c"
  # ADR-081 — adaptive CSI mesh firmware kernel
  "rv_radio_ops_esp32.c"
  "rv_feature_state.c"
  "rv_mesh.c"
  "adaptive_controller.c"
)

# ESP-IDF 6.x component requirements
set(REQUIRES 
  esp_wifi
  esp_netif
  nvs_flash
  esp_event
  esp_timer
  freertos
  lwip
  esp_http_client
  esp_http_server
  esp_https_ota
  app_update
  bootloader_support
  esp_adc
  driver
  mbedtls
  esp_hw_support
)

# Optional components
if(CONFIG_DISPLAY_ENABLE)
  list(APPEND REQUIRES esp_lcd lvgl)
endif()

if(CONFIG_CSI_MOCK_ENABLED)
  # Mock CSI for testing
endif()

# Add Minino BSP for board-specific initialization
list(APPEND REQUIRES minino_bsp)

# ADR-061: Mock CSI generator for QEMU testing + ADR-081 mock radio binding
if(CONFIG_CSI_MOCK_ENABLED)
  list(APPEND SRCS "mock_csi.c" "rv_radio_ops_mock.c")
endif()

# ADR-045: Display support (I2C OLED for Minino)
if(CONFIG_DISPLAY_ENABLE)
  list(APPEND SRCS "display_hal.c" "display_ui.c" "display_task.c")
  list(APPEND REQUIRES esp_lcd lvgl)
endif()

idf_component_register(
  SRCS ${SRCS}
  INCLUDE_DIRS "."
  REQUIRES ${REQUIRES}
  PRIV_REQUIRES
)
