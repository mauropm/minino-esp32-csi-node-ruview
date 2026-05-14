# Minino ESP32-C6 Migration Guide

## Overview

This firmware has been migrated to run on the **Electronic Cats Minino** hardware platform using the **ESP32-C6** microcontroller and **ESP-IDF v6.x** framework.

**Original Target:** ESP32-S3  
**New Target:** ESP32-C6 (RISC-V @ 160MHz)  
**Framework:** ESP-IDF v6.x  
**Board:** [Electronic Cats Minino](https://github.com/ElectronicCats/Minino)

---

## Hardware Support Matrix

| Peripheral | ESP32-S3 | ESP32-C6 (Minino) | Status |
|------------|----------|-------------------|--------|
| **WiFi** | WiFi 4 (802.11n) | WiFi 6 (802.11ax) | ✅ Full Support |
| **BLE** | BLE 5.0 | BLE 5.3 | ✅ Full Support |
| **CSI** | ✅ | ✅ | ✅ Full Support |
| **OLED Display** | SPI/I2C | I2C (0.91" 128x64) | ✅ Supported |
| **SD Card** | SDIO/SPI | SPI | ✅ Supported |
| **GPS** | UART | UART | ✅ Supported |
| **Buttons** | BOOT, RESET | BOOT, RESET | ✅ Supported |
| **LEDs** | GPIO | GPIO8 | ✅ Supported |
| **ADC** | ✅ | ✅ | ✅ Battery monitoring |
| **Thread** | ❌ | ✅ | ⚠️ Future |
| **Zigbee** | ❌ | ✅ | ⚠️ Future |

---

## Quick Start

### Prerequisites

1. **ESP-IDF v6.x** installed
2. **Electronic Cats Minino** board
3. **USB-C cable** for programming

More info in the file MININO-Requirements.md

### Build Instructions

```bash
# Navigate to project directory
cd minino-esp32-csi-node

# Set target to ESP32-C6
idf.py set-target esp32c6

# Build the project
idf.py build
```

### Flash Instructions

```bash
# Flash via USB
idf.py -p /dev/ttyACM0 flash

# Or on Windows
idf.py -p COM3 flash
```

### Monitor Output

```bash
# Monitor serial output
idf.py -p /dev/ttyACM0 monitor
```

---

## Pin Mapping (Minino BSP)

| Function | GPIO | Notes |
|----------|------|-------|
| **LED Status** | GPIO8 | Output |
| **BOOT Button** | GPIO9 | Input (pull-up) |
| **RESET Button** | GPIO1 | Input |
| **I2C SDA** | GPIO1 | OLED Display |
| **I2C SCL** | GPIO2 | OLED Display |
| **SPI SCLK** | GPIO4 | SD Card |
| **SPI MOSI** | GPIO5 | SD Card |
| **SPI MISO** | GPIO6 | SD Card |
| **SPI CS (SD)** | GPIO7 | SD Card |
| **UART TX** | GPIO3 | GPS Module |
| **UART RX** | GPIO4 | GPS Module |
| **VBUS Detect** | GPIO10 | USB Power |
| **Battery ADC** | GPIO5 | ADC1_CH5 |

---

## Configuration

### NVS Provisioning

Use the provision script to configure WiFi credentials:

```bash
python scripts/provision.py --port /dev/ttyACM0 \
  --ssid "YourWiFi" \
  --password "YourPassword" \
  --target-ip 192.168.1.100
```

### Kconfig Options

Run `idf.py menuconfig` to access:
- WiFi credentials
- Node ID
- Edge processing tier
- Target IP/port
- Channel hopping settings

---

## Hardware-Specific Notes

### WiFi 6 Features

The ESP32-C6 supports WiFi 6 (802.11ax) features:
- Target Wake Time (TWT)
- OFDMA
- Improved power efficiency

### Power Management

The Minino supports multiple power modes:
- **Active Mode:** Full performance @ 160MHz
- **Light Sleep:** WiFi maintain, reduced CPU
- **Deep Sleep:** RTC only, GPIO wakeup

### Display (OLED)

The Minino includes a 0.91" OLED display:
- **Interface:** I2C (0x3C)
- **Resolution:** 128x64
- **Driver:** SSD1306 compatible

Enable with: `CONFIG_DISPLAY_ENABLE=y`

### SD Card

SD card access via SPI:
- **Max Frequency:** 20MHz
- **Interface:** SPI2_HOST
- **CS Pin:** GPIO7

---

## Migration Changes

### ESP-IDF 6.x API Updates

1. **I2C Driver:** Migrated to new I2C master API
2. **ADC Driver:** Using esp_adc calibration
3. **Event Loop:** Using esp_event system
4. **WiFi:** esp_netif integration

### Removed Features

- ESP32-S3 specific GPIO configurations
- PSRAM dependencies (Minino has no PSRAM)
- Some display interfaces (SPI display removed)

### New Features

- Minino BSP layer
- ESP32-C6 specific power management
- WiFi 6 optimizations
- RISC-V specific optimizations

---

## Known Limitations

1. **No PSRAM:** Minino has no external PSRAM, only internal SRAM
2. **Single Antenna:** ESP32-C6 has internal antenna only
3. **ADC Resolution:** Limited to 12-bit on ESP32-C6
4. **GPIO Count:** Fewer GPIOs available than ESP32-S3

---

## Troubleshooting

### Build Fails

Ensure ESP-IDF v6.x is installed and target is set:
```bash
idf.py set-target esp32c6
idf.py fullclean
idf.py build
```

### WiFi Not Connecting

1. Check NVS credentials
2. Verify WiFi channel is valid (1-13 for 2.4GHz)
3. Check antenna connection

### Display Not Working

1. Verify I2C pins (GPIO1, GPIO2)
2. Check display address (0x3C)
3. Enable in menuconfig: `CONFIG_DISPLAY_ENABLE=y`

---

## Hardware Support Status

### Fully Supported ✅
- [x] WiFi Station Mode
- [x] WiFi CSI Collection
- [x] BLE 5.3
- [x] OLED Display (I2C)
- [x] SD Card (SPI)
- [x] UART (GPS)
- [x] Buttons (BOOT, RESET)
- [x] LED (GPIO8)
- [x] Battery Monitoring (ADC)
- [x] USB VBUS Detect
- [x] Deep Sleep
- [x] OTA Updates

### Partially Supported ⚠️
- [ ] Thread Protocol (future)
- [ ] Zigbee (future)
- [ ] Matter (future)

### Not Supported ❌
- [ ] PSRAM (hardware limitation)
- [ ] External antenna (hardware limitation)
- [ ] SDIO mode (SPI only)

---

## Performance Notes

### Memory Usage

| Component | Flash | RAM |
|-----------|-------|-----|
| Firmware | ~950 KB | ~35 KB |
| WASM Modules | ~100 KB | ~160 KB |
| FreeRTOS | - | ~20 KB |
| **Total** | **~1 MB** | **~215 KB** |

### ESP32-C6 Resources

- **Flash:** 8 MB (typical)
- **SRAM:** ~400 KB total
- **CPU:** RISC-V @ 160MHz
- **WiFi:** 802.11ax (WiFi 6)

---

## References

- [Minino Hardware Repository](https://github.com/ElectronicCats/Minino)
- [ESP32-C6 Technical Reference](https://www.espressif.com/en/products/socs/esp32-c6)
- [ESP-IDF v6 Documentation](https://docs.espressif.com/projects/esp-idf/en/latest/)

---

## License

This firmware is dual-licensed under MIT OR Apache-2.0.

Hardware design by Electronic Cats under CERN OHL v1.2.
