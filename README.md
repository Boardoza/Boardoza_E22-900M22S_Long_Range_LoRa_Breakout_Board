# Boardoza E22-900M22S LoRa® Breakout Board

The **Boardoza E22-900M22S Breakout Board** is a compact long-range wireless communication board based on the **Ebyte E22-900M22S** LoRa® module. Powered by the **Semtech SX1262** RF transceiver, it supports both **LoRa®** and **FSK(Frequency Shift Keying)** modulation, enabling reliable low-power wireless communication over the **850–930 MHz ISM frequency range**, including the widely used **868 MHz** and **915 MHz** bands. The onboard **32 MHz TCXO (Temperature Compensated Crystal Oscillator)** provides excellent frequency stability, making it suitable for narrowband communication and outdoor deployments.

The breakout board exposes the module's **SPI interface**, RF control signals, and status pins through standard headers, allowing easy integration with development platforms such as **STM32**, **ESP32**, **Arduino, and other 3.3V-compatible microcontrollers**. With its SMA antenna connector, industrial operating temperature range, and long communication distance, this board is ideal for **IoT gateways, smart agriculture, telemetry, environmental monitoring, industrial automation, and wireless sensor networks**.

---

## Key Features

- **Long-Range LoRa® Communication:** Based on the Semtech SX1262 transceiver for reliable long-distance wireless links.
- **868 / 915 MHz ISM Band Support:** Compatible with license-free ISM bands used worldwide.
- **High RF Output Power:** Up to **22 dBm (160 mW)** transmit power with software-adjustable output levels.
- **LoRa® and FSK Modulation Support:** Provides air data rates from 0.018 to 62.5 kbps in LoRa® mode and up to 300 kbps in FSK mode.
- **Stable RF Performance:** Integrated **32 MHz TCXO** improves frequency stability over temperature.
- **Easy MCU Integration:** Standard **4-wire SPI interface** with dedicated control and status signals.
- **SMA Antenna Connector:** Ready for external antennas with improved RF performance.
- **Ideal for IoT Applications:** Suitable for wireless sensor networks, smart agriculture, telemetry, industrial monitoring, and remote data acquisition.

---

## Technical Specifications

**Model:** E22-900M22S   
**Manufacturer:** Boardoza    
**Manufacturer IC:** Chengdu Ebyte Electronic Technology Co., Ltd.   
**RF Transceiver:** Semtech SX1262  
**Input Voltage:** 3.3V – 5.5V     
**Interface:** SPI  
**Frequency Range:** 850 – 930 MHz     
**Maximum RF Output Power:** 22 dBm (160 mW)  
**Maximum Air Data Rate (LoRa®):** 0.018 to 62.5 kbps  
**Maximum Air Data Rate (FSK):** 300 kbps  
**FIFO Buffer:** 256 Bytes  
**Oscillator:** High-Precision 32 MHz TCXO  
**Antenna Connector:** SMA Female  
**Operating Temperature:** -40°C to +85°C  
**Board Dimensions:** 40 mm × 20 mm

---

## Board Pinout

### ( J1 ) SPI Interface

| Pin Number | Pin Name | Description |
|:---:|:---:|---|
| 1 | VCC | Power Input |
| 2 | MISO | SPI Data Output |
| 3 | MOSI | SPI Data Input |
| 4 | SCK | SPI Clock |
| 5 | nCS | SPI Chip Select (Active Low) |
| 6 | GND | Ground |

### ( J2 ) Control Signals

| Pin Number | Pin Name | Description |
|:---:|:---:|---|
| 1 | NRST | Reset Input (Active Low) |
| 2 | BUSY | Module Busy Status Output |
| 3 | DIO1 | Configurable Digital I/O |

### ( J3 ) RF Switch & Control

| Pin Number | Pin Name | Description |
|:---:|:---:|---|
| 1 | RXEN | RF Receive Enable (Active High) |
| 2 | TXEN | RF Transmit Enable (Active High) |
| 3 | DIO2 | Configurable Digital I/O / RF Switch Control |

### ( J4 ) RF Connector

| Connector | Description |
|:---:|---|
| SMA Female | External 50Ω Antenna Connection |

---

## Board Dimensions

<img src="./assets/E22_900M22S Dimensions.png" alt="Board Dimensions" width="450"/>

---

## Step Files

[Boardoza E22-900M22S.step](./assets/E22_900M22S%20Step.step)

---

## Datasheet

[E22-900M22S Datasheet.pdf](./assets/E22-900M22S%20Datasheet.pdf)

---

## Version History

- V1.0.0 - Initial Release

---

## Support

- If you have any questions or need support, please contact **support@boardoza.com**

---

## **License**

This repository contains both hardware and software components:

### **Hardware Design**

[![CC BY-SA 4.0][cc-by-sa-shield]][cc-by-sa]

All hardware design files are licensed under [Creative Commons Attribution-ShareAlike 4.0 International License][cc-by-sa].

[cc-by-sa]: http://creativecommons.org/licenses/by-sa/4.0/
[cc-by-sa-shield]: https://img.shields.io/badge/License-CC%20BY--SA%204.0-lightgrey.svg

### **Software/Firmware**

[![BSD-3-Clause][bsd-shield]][bsd]

All software and firmware are licensed under [BSD 3-Clause License][bsd].

[bsd]: https://opensource.org/licenses/BSD-3-Clause
[bsd-shield]: https://img.shields.io/badge/License-BSD%203--Clause-blue.svg
