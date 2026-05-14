# Minino ESP32-C6 CSI Node Firmware
# Requires ESP-IDF v6.x
# Target: Electronic Cats Minino (ESP32-C6)
cmake_minimum_required(VERSION 3.16)

# Set target to ESP32-C6 (RISC-V)
# This will be overridden by sdkconfig.defaults if present
set(IDF_TARGET "esp32c6" CACHE STRING "Target ESP32-C6" FORCE)

# Add Minino BSP component
set(EXTRA_COMPONENT_DIRS "${CMAKE_CURRENT_LIST_DIR}/components")

# Read firmware version from version.txt
file(STRINGS "${CMAKE_CURRENT_LIST_DIR}/version.txt" PROJECT_VER LIMIT_COUNT 1)
string(STRIP "${PROJECT_VER}" PROJECT_VER)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(minino-csi-node VERSION ${PROJECT_VER})
