#!/bin/bash
# Minino ESP32-C6 Flash Script
# Flashes firmware to Electronic Cats Minino

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

# Default serial port
SERIAL_PORT=""
BAUD_RATE=460800

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

# Help function
show_help() {
    echo "Usage: $0 [OPTIONS]"
    echo ""
    echo "Options:"
    echo "  -p, --port PORT    Serial port (default: auto-detect)"
    echo "  -b, --baud BAUD    Baud rate (default: 460800)"
    echo "  -h, --help         Show this help"
    echo ""
    echo "Examples:"
    echo "  ./flash_minino.sh -p /dev/ttyACM0"
    echo "  ./flash_minino.sh -p COM3"
}

# Parse arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -p|--port)
            SERIAL_PORT="$2"
            shift 2
            ;;
        -b|--baud)
            BAUD_RATE="$2"
            shift 2
            ;;
        -h|--help)
            show_help
            exit 0
            ;;
        *)
            echo "Unknown option: $1"
            show_help
            exit 1
            ;;
    esac
done

# Check if build exists
if [ ! -f "build/minino-csi-node.bin" ]; then
    echo -e "${RED}Error: Firmware not built${NC}"
    echo "Please run ./build_minino.sh first"
    exit 1
fi

# Flash
echo -e "${GREEN}Flashing Minino...${NC}"
if [ -n "$SERIAL_PORT" ]; then
    idf.py -p "$SERIAL_PORT" -b "$BAUD_RATE" flash
else
    idf.py -b "$BAUD_RATE" flash
fi

echo -e "${GREEN}Flash complete!${NC}"
echo ""
echo "To monitor serial output:"
if [ -n "$SERIAL_PORT" ]; then
    echo "  idf.py -p $SERIAL_PORT monitor"
else
    echo "  idf.py monitor"
fi
