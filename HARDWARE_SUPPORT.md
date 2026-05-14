# Minino Hardware Support Matrix

## Overview

This document describes the hardware support status for the Electronic Cats Minino platform with ESP32-C6.

**Board:** Electronic Cats Minino  
**MCU:** ESP32-C6 (RISC-V @ 160MHz)  
**Flash:** 8MB (typical)  
**Framework:** ESP-IDF v6.x

---

## Peripheral Support Status

### WiFi & Wireless

| Feature | Status | Notes |
|---------|--------|-------|
| **WiFi 6 (802.11ax)** | ✅ Full | 2.4GHz only |
| **WiFi CSI** | ✅ Full | Channel State Information |
| **WiFi Station** | ✅ Full | Client mode |
| **WiFi AP** | ✅ Full | Access Point mode |
| **BLE 5.3** | ✅ Full | Bluetooth Low Energy |
| **Thread** | ⚠️ Planned | Future release |
| **Zigbee** | ⚠️ Planned | Future release |
| **Matter** | ⚠️ Planned | Future release |

### GPIO & Interfaces

| Interface | Status | Pins | Notes |
|-----------|--------|------|-------|
| **GPIO** | ✅ Full | Multiple | LED, buttons |
| **I2C** | ✅ Full | GPIO1, GPIO2 | OLED, sensors |
| **SPI** | ✅ Full | GPIO4-7 | SD card |
| **UART** | ✅ Full | GPIO3, GPIO4 | GPS module |
| **ADC** | ✅ Full | GPIO5 | Battery voltage |
| **PWM** | ✅ Full | Multiple | LED, motors |

### Display & Input

| Component | Status | Interface | Notes |
|-----------|--------|-----------|-------|
| **OLED Display** | ✅ Full | I2C | 0.91" 128x64 |
| **BOOT Button** | ✅ Full | GPIO9 | Input with pull-up |
| **RESET Button** | ✅ Full | GPIO1 | Hardware reset |
| **Status LED** | ✅ Full | GPIO8 | Green LED |

### Storage

| Component | Status | Interface | Notes |
|-----------|--------|-----------|-------|
| **Internal Flash** | ✅ Full | SPI | 8MB |
| **SD Card** | ✅ Full | SPI | Up to 32GB |
| **NVS** | ✅ Full | Internal | Configuration storage |

### Sensors

| Sensor Type | Status | Interface | Notes |
|-------------|--------|-----------|-------|
| **CSI (WiFi)** | ✅ Full | Internal | Human sensing |
| **GPS** | ✅ Full | UART | 9600 baud |
| **Battery** | ✅ Full | ADC | Voltage monitoring |
| **USB VBUS** | ✅ Full | GPIO10 | Power detect |

---

## Pin Allocation Details

### GPIO Usage Summary

| GPIO | Function | Direction | Notes |
|------|----------|-----------|-------|
| GPIO0 | Reserved | - | Internal |
| GPIO1 | I2C SCL / RESET | Input/Output | Display, Button |
| GPIO2 | I2C SDA | Output | Display |
| GPIO3 | UART TX | Output | GPS TX |
| GPIO4 | SPI SCLK | Output | SD Card |
| GPIO5 | SPI MOSI / ADC | Output/Input | SD, Battery |
| GPIO6 | SPI MISO | Input | SD Card |
| GPIO7 | SPI CS | Output | SD Card Select |
| GPIO8 | LED | Output | Status LED |
| GPIO9 | BOOT | Input | Boot button |
| GPIO10 | VBUS Detect | Input | USB power |

### Internal Connections

| Signal | Description |
|--------|-------------|
| **WiFi Antenna** | Internal (on-module) |
| **Crystal** | 40MHz main oscillator |
| **Flash** | 8MB SPI flash |
| **PSRAM** | Not available |

---

## Power Management

### Power Modes

| Mode | Current | Wake Sources | Notes |
|------|---------|--------------|-------|
| **Active** | ~100mA | - | Full operation |
| **Light Sleep** | ~1mA | GPIO, Timer | WiFi maintained |
| **Deep Sleep** | ~100µA | GPIO, Timer | RTC only |
| **Hibernate** | ~10µA | EXT0/EXT1 | Minimal power |

### Battery Monitoring

- **ADC Channel:** ADC1_CH5 (GPIO5)
- **Resolution:** 12-bit
- **Range:** 0-3.3V
- **Calibration:** eFuse based

---

## Known Limitations

### Hardware Limitations

1. **No PSRAM:** Minino does not have external PSRAM
   - Impact: Limited heap size (~400KB total SRAM)
   - Mitigation: Careful memory management

2. **Single Antenna:** Internal antenna only
   - Impact: No antenna diversity
   - Mitigation: Optimize placement

3. **ADC Resolution:** 12-bit maximum
   - Impact: Limited precision for analog sensing
   - Mitigation: Oversampling if needed

4. **SPI Shared:** SD card shares SPI bus
   - Impact: Cannot use SPI display and SD simultaneously
   - Mitigation: I2C display recommended

### Software Limitations

1. **WiFi 6 Features:** Not all WiFi 6 features enabled
   - Current: Basic 802.11ax support
   - Future: OFDMA, TWT optimization

2. **Thread/Zigbee:** Not yet implemented
   - Current: WiFi + BLE only
   - Future: Full Matter support

3. **Display Drivers:** Limited to I2C OLED
   - Current: SSD1306 compatible
   - Future: SPI display support

---

## Performance Specifications

### WiFi Performance

| Metric | Value | Notes |
|--------|-------|-------|
| **RSSI Range** | -90 to -10 dBm | Typical indoor |
| **CSI Rate** | ~20-100 Hz | Configurable |
| **UDP Throughput** | ~5 Mbps | Single stream |
| **Connection Time** | <2s | Cold start |

### Memory Usage

| Component | Flash | RAM |
|-----------|-------|-----|
| **Firmware** | ~950 KB | - |
| **Runtime** | - | ~35 KB |
| **WASM Modules** | ~100 KB | ~160 KB |
| **FreeRTOS** | - | ~20 KB |
| **Total** | ~1 MB | ~215 KB |

### Power Consumption

| State | Current (@3.3V) | Notes |
|-------|-----------------|-------|
| **TX (WiFi)** | ~200mA | Peak transmit |
| **RX (WiFi)** | ~100mA | Receiving |
| **Idle** | ~50mA | WiFi connected |
| **Light Sleep** | ~1mA | WiFi maintained |
| **Deep Sleep** | ~100µA | RTC only |

---

## Environmental Specifications

### Operating Conditions

| Parameter | Range | Notes |
|-----------|-------|-------|
| **Temperature** | -20°C to +70°C | Industrial |
| **Humidity** | 10% to 90% | Non-condensing |
| **Voltage** | 3.0V to 3.6V | Normal operation |

---

## Compliance

| Standard | Status | Notes |
|----------|--------|-------|
| **FCC** | ✅ | ESP32-C6 certified |
| **CE** | ✅ | ESP32-C6 certified |
| **RoHS** | ✅ | Lead-free |
| **WiFi Alliance** | ✅ | WiFi 6 certified |

---

## Revision History

| Version | Date | Changes |
|---------|------|---------|
| 1.0.0 | 2025-01 | Initial release |
| 1.0.1 | 2025-02 | Added Thread support notes |

---

## References

- [Minino Hardware Repository](https://github.com/ElectronicCats/Minino)
- [ESP32-C6 Datasheet](https://www.espressif.com/sites/default/files/esp32-c6_datasheet_en.pdf)
- [ESP-IDF Documentation](https://docs.espressif.com/projects/esp-idf/en/latest/)
