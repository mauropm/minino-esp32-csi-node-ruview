#!/bin/bash
# Minino ESP32-C6 Build Script
# Builds firmware for Electronic Cats Minino

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${GREEN}==================================${NC}"
echo -e "${GREEN}Minino ESP32-C6 Build Script${NC}"
echo -e "${GREEN}==================================${NC}"
echo ""

# Check if IDF_PATH is set
if [ -z "$IDF_PATH" ]; then
    echo -e "${RED}Error: ESP-IDF environment not set${NC}"
    echo "Please source the ESP-IDF export script first:"
    echo "  source \$HOME/esp/esp-idf/export.sh"
    exit 1
fi

# Check ESP-IDF version (ESP-IDF v6+ uses different version detection)
if [ -f "$IDF_PATH/tools/cmake/version.cmake" ]; then
    IDF_VERSION=$(grep "set\(IDF_VERSION_MAJOR" $IDF_PATH/tools/cmake/version.cmake | cut -d'"' -f2)
    if [ -n "$IDF_VERSION" ] && [ "$IDF_VERSION" -lt 6 ]; then
        echo -e "${YELLOW}Warning: ESP-IDF version should be 6.x or higher${NC}"
    fi
fi

# Clean build directory (optional)
if [ "$1" == "clean" ]; then
    echo -e "${YELLOW}Cleaning build directory...${NC}"
    rm -rf build sdkconfig
fi

# Set target to ESP32-C6
echo -e "${YELLOW}Setting target to ESP32-C6...${NC}"
idf.py set-target esp32c6

# Build
echo -e "${YELLOW}Building firmware...${NC}"
idf.py build

# Show build status
if [ -f "build/minino-csi-node.bin" ]; then
    SIZE=$(ls -lh build/minino-csi-node.bin | awk '{print $5}')
    echo -e "${GREEN}==================================${NC}"
    echo -e "${GREEN}Build successful!${NC}"
    echo -e "${GREEN}Binary size: ${SIZE}${NC}"
    echo -e "${GREEN}==================================${NC}"
    echo ""
    echo "To flash the firmware:"
    echo "  idf.py -p /dev/ttyACM0 flash"
    echo ""
    echo "To monitor:"
    echo "  idf.py -p /dev/ttyACM0 monitor"
else
    echo -e "${RED}Build failed!${NC}"
    exit 1
fi
