# Minino ESP32-C6 CSI Node - Requirements

## Overview

This document lists all hardware and software requirements for building and running the ESP32-C6 CSI Node firmware on the Electronic Cats Minino platform.

---

## Hardware Requirements

### Required Hardware

| Component | Specification | Notes |
|-----------|---------------|-------|
| **Board** | Electronic Cats Minino | ESP32-C6 based board |
| **Flash Memory** | 8MB minimum | Required for OTA updates |
| **USB Cable** | USB-C to USB-A/C | For programming and power |
| **Computer** | Windows/macOS/Linux | For development and flashing |

**Board Purchase Links:**
- [Electronic Cats Minino Store](https://electroniccats.com/store/minino/)
- [Official GitHub Repository](https://github.com/ElectronicCats/Minino)

### Optional Hardware

| Component | Specification | Notes |
|-----------|---------------|-------|
| **OLED Display** | 0.91" I2C OLED (SSD1306) | 128x64, I2C 0x3C |
| **SD Card** | 1-32GB microSD | For data logging |
| **GPS Module** | UART GPS (9600 baud) | For location tagging |
| **Antenna** | 2.4GHz WiFi antenna | Improved range (if not internal) |

### Hardware Specifications

**Minino Board Specs:**
- **MCU:** ESP32-C6 (RISC-V @ 160MHz)
- **Flash:** 8MB (typical)
- **WiFi:** 802.11ax (WiFi 6)
- **BLE:** 5.3
- **GPIO:** Limited pins available
- **I2C:** 1 bus (GPIO1, GPIO2)
- **SPI:** 1 bus (GPIO4-7)
- **UART:** 1 bus (GPIO3, GPIO4)
- **ADC:** 12-bit

**Pin Mapping:**
| Function | GPIO | Notes |
|----------|------|-------|
| LED Status | GPIO8 | Output |
| BOOT Button | GPIO9 | Input |
| I2C SDA | GPIO1 | Display/Sensors |
| I2C SCL | GPIO2 | Display/Sensors |
| SPI SCLK | GPIO4 | SD Card |
| SPI MOSI | GPIO5 | SD Card |
| SPI MISO | GPIO6 | SD Card |
| SPI CS | GPIO7 | SD Card Select |
| UART TX | GPIO3 | GPS TX |
| UART RX | GPIO4 | GPS RX |

---

## Software Requirements

### Required Software

| Software | Version | Purpose | Download |
|----------|---------|---------|----------|
| **ESP-IDF** | v6.0+ | Main development framework | [Espressif Installer](https://docs.espressif.com/projects/esp-idf/en/latest/esp32c6/get-started/install-operating.html) |
| **Python** | 3.10+ | Build system and scripts | [python.org](https://www.python.org/) |
| **Git** | Latest | Version control | [git-scm.com](https://git-scm.com/) |
| **CMake** | 3.16+ | Build system | [cmake.org](https://cmake.org/) |

### Recommended Software

| Software | Version | Purpose |
|----------|---------|---------|
| **VS Code** | Latest | Code editor |
| **ESP-IDF Extension** | Latest | VS Code integration |
| **Serial Terminal** | Any | Monitoring (minicom, screen, PuTTY) |
| **Docker** | Optional | Containerized builds |

---

## Installation Steps

### 1. Install ESP-IDF v6.x

#### macOS/Linux

```bash
# Install prerequisites (macOS)
brew install cmake ninja dfu-util

# Install ESP-IDF
mkdir -p ~/esp
cd ~/esp
git clone -b v6.0 https://github.com/espressif/esp-idf.git
cd esp-idf
./install.sh esp32c6

# Set up environment
. ./export.sh
```

#### Windows

```powershell
# Download ESP-IDF Tools Installer
# https://dl.espressif.com/dl/esp-idf/esp-idf-tools-setup-offline-6.0.exe

# Run installer and select:
# - ESP-IDF v6.0
# - ESP32-C6 support
# - Python 3.x
# - Git
```

### 2. Verify Installation

```bash
# Check ESP-IDF version
idf.py --version

# Should output: ESP-IDF v6.x.x

# Check target support
idf.py set-target esp32c6
```

### 3. Install Python Dependencies

```bash
# ESP-IDF will install most dependencies automatically
# Additional Python packages if needed:
pip install pyserial cryptography
```

---

## Build Requirements

### Disk Space

| Component | Size |
|-----------|------|
| ESP-IDF Framework | ~10 GB |
| Toolchain | ~2 GB |
| Project Source | ~50 MB |
| Build Output | ~500 MB |
| **Total Recommended** | **15 GB** |

### Memory (RAM)

| Operation | Minimum | Recommended |
|-----------|---------|-------------|
| Build | 4 GB | 8 GB |
| Flash/Monitor | 2 GB | 4 GB |

---

## Configuration Requirements

### Environment Variables

The following environment variables must be set:

```bash
# ESP-IDF installation path
export IDF_PATH=~/esp/esp-idf

# Python virtual environment
source $IDF_PATH/export.sh

# Serial port (optional, can be specified per-command)
export ESPPORT=/dev/cu.usbmodem21101  # macOS
# export ESPPORT=/dev/ttyUSB0         # Linux
# export ESPPORT=COM3                 # Windows
```

### SDK Configuration

The following configuration options are required:

```ini
CONFIG_IDF_TARGET="esp32c6"
CONFIG_ESPTOOLPY_FLASHSIZE_8MB=y
CONFIG_PARTITION_TABLE_CUSTOM=y
CONFIG_ESP_WIFI_CSI_ENABLED=y
CONFIG_DISPLAY_ENABLE=n  # Disable display to avoid I2C conflicts
```

---

## Build Commands

### Quick Build

```bash
# Navigate to project
cd minino-esp32-csi-node

# Set target and build
idf.py set-target esp32c6
idf.py build
```

### Using Build Script

```bash
# Use provided build script
./build_minino.sh
```

### Clean Build

```bash
# Full clean and rebuild
idf.py fullclean
idf.py set-target esp32c6
idf.py build
```

---

## Flash Requirements

### Serial Connection

| Parameter | Value |
|-----------|-------|
| **Baud Rate** | 460800 (default) |
| **Data Bits** | 8 |
| **Stop Bits** | 1 |
| **Parity** | None |
| **Flow Control** | None |

### Flash Commands

```bash
# Flash via USB
idf.py -p /dev/cu.usbmodem21101 flash

# Or use the provided script
./flash_minino.sh -p /dev/cu.usbmodem21101
```

### Monitor Output

```bash
# Monitor serial output
idf.py -p /dev/cu.usbmodem21101 monitor

# Exit monitor: Ctrl+]
```

---

## Network Requirements

### WiFi Network

| Requirement | Details |
|-------------|---------|
| **Frequency** | 2.4 GHz (802.11 b/g/n/ax) |
| **Security** | WPA2-PSK (recommended) |
| **Channel** | 1-13 (2.4 GHz band) |

### Network Configuration

The following network settings must be configured via NVS:

- **SSID:** Your WiFi network name
- **Password:** Network password
- **Target IP:** Aggregator server IP
- **Target Port:** UDP port (default: 5005)

### Provisioning

```bash
# Provision WiFi credentials
python scripts/provision.py --port /dev/cu.usbmodem21101 \
  --ssid "YourWiFi" \
  --password "YourPassword" \
  --target-ip 192.168.1.100
```

---

## Development Environment

### Recommended Setup

**Operating System:** macOS 12+, Ubuntu 20.04+, or Windows 10+

**Editor:** VS Code with ESP-IDF Extension

**Terminal:** 
- macOS: Terminal.app or iTerm2
- Linux: GNOME Terminal or Konsole
- Windows: PowerShell or WSL2

### VS Code Extensions

1. **ESP-IDF Extension** (espressif.esp-idf-extension)
2. **C/C++** (ms-vscode.cpptools)
3. **CMake Tools** (ms-vscode.cmake-tools)

---

## Troubleshooting Requirements

### Common Issues

| Issue | Solution |
|-------|----------|
| **Build fails** | Check ESP-IDF version (must be 6.0+) |
| **Flash fails** | Check USB cable and port permissions |
| **WiFi won't connect** | Verify SSID/password in NVS |
| **I2C conflict** | Disable CONFIG_DISPLAY_ENABLE |

### Debug Tools

```bash
# Check ESP-IDF installation
idf.py install-check

# Check serial ports
# macOS/Linux:
ls /dev/cu.*

# Windows:
Get-ItemProperty HKLM:\SYSTEM\CurrentControlSet\Enum\FTDIBUS\*\* | Select FriendlyName
```

---

## Version Compatibility Matrix

| ESP-IDF Version | Status | Notes |
|-----------------|--------|-------|
| v5.x | ❌ Not Supported | Old I2C API conflicts |
| v6.0.x | ✅ Supported | Minimum required version |
| v6.1+ | ✅ Supported | Recommended |
| v7.0+ | ⚠️ Untested | May work, not verified |

---

## Performance Requirements

### Build Performance

| Metric | Minimum | Recommended |
|--------|---------|-------------|
| Build Time | < 10 min | < 5 min |
| Flash Time | < 30 sec | < 15 sec |

### Runtime Performance

| Metric | Value |
|--------|-------|
| **CSI Rate** | ~20-100 Hz |
| **WiFi Latency** | < 100 ms |
| **Power Consumption** | ~100mA (active) |

---

## Security Requirements

### Recommended Security Settings

```ini
# Enable secure boot (production)
CONFIG_SECURE_BOOT_ENABLE=y

# Enable flash encryption (production)
CONFIG_FLASH_ENCRYPTION_ENABLE=y

# Enable signature verification
CONFIG_WASM_VERIFY_SIGNATURE=y
```

### Development Mode

For development, security can be disabled:

```ini
CONFIG_SECURE_BOOT_ENABLE=n
CONFIG_FLASH_ENCRYPTION_ENABLE=n
CONFIG_WASM_VERIFY_SIGNATURE=n
```

---

## References

- [ESP-IDF Documentation](https://docs.espressif.com/projects/esp-idf/en/v6.0/)
- [Minino Hardware Repository](https://github.com/ElectronicCats/Minino)
- [ESP32-C6 Technical Reference](https://www.espressif.com/en/products/socs/esp32-c6)
- [ESP-IDF Installation Guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32c6/get-started/install-operating.html)

---

## Support

For issues or questions:
1. Check the [troubleshooting guide](TROUBLESHOOTING.md)
2. Review [ESP-IDF documentation](https://docs.espressif.com/projects/esp-idf/)
3. Check [Minino GitHub issues](https://github.com/ElectronicCats/Minino/issues)

---

**Last Updated:** 2025-01-14  
**ESP-IDF Version:** 6.0+  
**Target:** ESP32-C6 (Minino)
