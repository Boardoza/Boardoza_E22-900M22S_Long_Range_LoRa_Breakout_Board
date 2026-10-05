#include <SX126x_driver.h>

SPIClass* sx126x_spi = &SX126X_SPI;
uint32_t sx126x_spiFrequency = SX126X_SPI_FREQUENCY;
int8_t sx126x_nss = SX126X_PIN_NSS;
int8_t sx126x_busy = SX126X_PIN_BUSY;

/**
 * @brief Set the SPI object and frequency for SX126x communication.
 * @param SpiObject Reference to SPIClass object.
 * @param frequency SPI frequency in Hz.
 */
void sx126x_setSPI(SPIClass &SpiObject, uint32_t frequency)
{
    sx126x_spi = &SpiObject;
    sx126x_spiFrequency = frequency ? frequency : sx126x_spiFrequency;
}

/**
 * @brief Set the NSS (chip select) and BUSY pin numbers.
 * @param nss NSS pin number.
 * @param busy BUSY pin number.
 */
void sx126x_setPins(int8_t nss, int8_t busy)
{
    sx126x_nss = nss;
    sx126x_busy = busy;
}

/**
 * @brief Reset the SX126x device using the reset pin.
 * @param reset Reset pin number.
 */
void sx126x_reset(int8_t reset)
{
    pinMode(reset, OUTPUT);
    digitalWrite(reset, LOW);
    delayMicroseconds(500);
    digitalWrite(reset, HIGH);
    delayMicroseconds(100);
}

/**
 * @brief Initialize the SX126x device (set pin modes and begin SPI).
 */
void sx126x_begin()
{
    pinMode(sx126x_nss, OUTPUT);
    pinMode(sx126x_busy, INPUT);
    sx126x_spi->begin();
}

/**
 * @brief Check if the SX126x device is busy, with timeout.
 * @param timeout Timeout in milliseconds.
 * @return true if busy, false otherwise.
 */
bool sx126x_busyCheck(uint32_t timeout)
{
    uint32_t t = millis();
    while (digitalRead(sx126x_busy) == HIGH) if (millis() - t > timeout) return true;
    return false;
}

/**
 * @brief Put the SX126x device into sleep mode.
 * @param sleepConfig Sleep configuration value.
 */
void sx126x_setSleep(uint8_t sleepConfig)
{
    sx126x_transfer(0x84, &sleepConfig, 1);
}

/**
 * @brief Set the SX126x device to standby mode.
 * @param standbyConfig Standby configuration value.
 */
void sx126x_setStandby(uint8_t standbyConfig)
{
    sx126x_transfer(0x80, &standbyConfig, 1);
}

/**
 * @brief Set the SX126x device to frequency synthesis mode.
 */
void sx126x_setFs()
{
    sx126x_transfer(0xC1, NULL, 0);
}

/**
 * @brief Set the SX126x device to transmit mode with timeout.
 * @param timeout Timeout value.
 */
void sx126x_setTx(uint32_t timeout)
{
    uint8_t buf[3];
    buf[0] = timeout >> 16;
    buf[1] = timeout >> 8;
    buf[2] = timeout;
    sx126x_transfer(0x83, buf, 3);
}

/**
 * @brief Set the SX126x device to receive mode with timeout.
 * @param timeout Timeout value.
 */
void sx126x_setRx(uint32_t timeout)
{
    uint8_t buf[3];
    buf[0] = timeout >> 16;
    buf[1] = timeout >> 8;
    buf[2] = timeout;
    sx126x_transfer(0x82, buf, 3);
}

/**
 * @brief Stop timer on preamble detection.
 * @param enable Enable or disable (1 or 0).
 */
void sx126x_stopTimerOnPreamble(uint8_t enable)
{
    sx126x_transfer(0x9F, &enable, 1);
}

/**
 * @brief Set RX duty cycle (periodic RX and sleep).
 * @param rxPeriod RX period.
 * @param sleepPeriod Sleep period.
 */
void sx126x_setRxDutyCycle(uint32_t rxPeriod, uint32_t sleepPeriod)
{
    uint8_t buf[6];
    buf[0] = rxPeriod >> 16;
    buf[1] = rxPeriod >> 8;
    buf[2] = rxPeriod;
    buf[3] = sleepPeriod >> 16;
    buf[4] = sleepPeriod >> 8;
    buf[5] = sleepPeriod;
    sx126x_transfer(0x94, buf, 6);
}

/**
 * @brief Set the SX126x device to CAD (Channel Activity Detection) mode.
 */
void sx126x_setCad()
{
    sx126x_transfer(0xC5, NULL, 0);
}

/**
 * @brief Set the SX126x device to transmit continuous wave mode.
 */
void sx126x_setTxContinuousWave()
{
    sx126x_transfer(0xD1, NULL, 0);
}

/**
 * @brief Set the SX126x device to transmit infinite preamble mode.
 */
void sx126x_setTxInfinitePreamble()
{
    sx126x_transfer(0xD2, NULL, 0);
}

/**
 * @brief Set the regulator mode.
 * @param modeParam Regulator mode parameter.
 */
void sx126x_setRegulatorMode(uint8_t modeParam)
{
    sx126x_transfer(0x96, &modeParam, 1);
}

/**
 * @brief Calibrate the SX126x device.
 * @param calibParam Calibration parameter.
 */
void sx126x_calibrate(uint8_t calibParam)
{
    sx126x_transfer(0x89, &calibParam, 1);
}

/**
 * @brief Calibrate the image rejection for a given frequency range.
 * @param freq1 First frequency calibration value.
 * @param freq2 Second frequency calibration value.
 */
void sx126x_calibrateImage(uint8_t freq1, uint8_t freq2)
{
    uint8_t buf[2];
    buf[0] = freq1;
    buf[1] = freq2;
    sx126x_transfer(0x98, buf, 2);
}

/**
 * @brief Set the power amplifier configuration.
 * @param paDutyCycle PA duty cycle.
 * @param hpMax High power max.
 * @param deviceSel Device selection.
 * @param paLut PA LUT value.
 */
void sx126x_setPaConfig(uint8_t paDutyCycle, uint8_t hpMax, uint8_t deviceSel, uint8_t paLut)
{
    uint8_t buf[4];
    buf[0] = paDutyCycle;
    buf[1] = hpMax;
    buf[2] = deviceSel;
    buf[3] = paLut;
    sx126x_transfer(0x95, buf, 4);
}

/**
 * @brief Set the RX/TX fallback mode.
 * @param fallbackMode Fallback mode value.
 */
void sx126x_setRxTxFallbackMode(uint8_t fallbackMode)
{
    sx126x_transfer(0x93, &fallbackMode, 1);
}

/**
 * @brief Write data to a register address.
 * @param address Register address.
 * @param data Pointer to data array.
 * @param nData Number of bytes to write.
 */
void sx126x_writeRegister(uint16_t address, uint8_t* data, uint8_t nData)
{
    uint8_t bufAdr[2] = {address >> 8, address};
    sx126x_transfer(0x0D, data, nData, bufAdr, 2, false);
}

/**
 * @brief Read data from a register address.
 * @param address Register address.
 * @param data Pointer to data array.
 * @param nData Number of bytes to read.
 */
void sx126x_readRegister(uint16_t address, uint8_t* data, uint8_t nData)
{
    uint8_t bufAdr[3] = {address >> 8, address, 0x00};
    sx126x_transfer(0x1D, data, nData, bufAdr, 3, true);
}

/**
 * @brief Write data to the transmit buffer.
 * @param offset Buffer offset.
 * @param data Pointer to data array.
 * @param nData Number of bytes to write.
 */
void sx126x_writeBuffer(uint8_t offset, uint8_t* data, uint8_t nData)
{
    uint8_t bufOfs[1] = {offset};
    sx126x_transfer(0x0E, data, nData, bufOfs, 1, false);
}

/**
 * @brief Read data from the receive buffer.
 * @param offset Buffer offset.
 * @param data Pointer to data array.
 * @param nData Number of bytes to read.
 */
void sx126x_readBuffer(uint8_t offset, uint8_t* data, uint8_t nData)
{
    uint8_t bufOfs[2] = {offset, 0x00};
    sx126x_transfer(0x1E, data, nData, bufOfs, 2, true);
}

/**
 * @brief Set DIO IRQ parameters for the device.
 * @param irqMask IRQ mask.
 * @param dio1Mask DIO1 mask.
 * @param dio2Mask DIO2 mask.
 * @param dio3Mask DIO3 mask.
 */
void sx126x_setDioIrqParams(uint16_t irqMask, uint16_t dio1Mask, uint16_t dio2Mask, uint16_t dio3Mask)
{
    uint8_t buf[8];
    buf[0] = irqMask >> 8;
    buf[1] = irqMask;
    buf[2] = dio1Mask >> 8;
    buf[3] = dio1Mask;
    buf[4] = dio2Mask >> 8;
    buf[5] = dio2Mask;
    buf[6] = dio3Mask >> 8;
    buf[7] = dio3Mask;
    sx126x_transfer(0x08, buf, 8);
}

/**
 * @brief Get the IRQ status.
 * @param irqStatus Pointer to store IRQ status.
 */
void sx126x_getIrqStatus(uint16_t* irqStatus)
{
    uint8_t buf[3];
    sx126x_transfer(0x12, buf, 3);
    *irqStatus = (buf[1] << 8) | buf[2];
}

/**
 * @brief Clear the IRQ status.
 * @param clearIrqParam IRQ clear parameter.
 */
void sx126x_clearIrqStatus(uint16_t clearIrqParam)
{
    uint8_t buf[2];
    buf[0] = clearIrqParam >> 8;
    buf[1] = clearIrqParam;
    sx126x_transfer(0x02, buf, 2);
}

/**
 * @brief Set DIO2 as RF switch control.
 * @param enable Enable or disable.
 */
void sx126x_setDio2AsRfSwitchCtrl(uint8_t enable)
{
    sx126x_transfer(0x9D, &enable, 1);
}

/**
 * @brief Set DIO3 as TCXO control with voltage and delay.
 * @param tcxoVoltage TCXO voltage.
 * @param delay Delay time.
 */
void sx126x_setDio3AsTcxoCtrl(uint8_t tcxoVoltage, uint32_t delay)
{
    uint8_t buf[4];
    buf[0] = tcxoVoltage;
    buf[1] = delay >> 16;
    buf[2] = delay >> 8;
    buf[3] = delay;
    sx126x_transfer(0x97, buf, 4);
}

/**
 * @brief Set the RF frequency.
 * @param rfFreq RF frequency value.
 */
void sx126x_setRfFrequency(uint32_t rfFreq)
{
    uint8_t buf[4];
    buf[0] = rfFreq >> 24;
    buf[1] = rfFreq >> 16;
    buf[2] = rfFreq >> 8;
    buf[3] = rfFreq;
    sx126x_transfer(0x86, buf, 4);
}

/**
 * @brief Set the packet type (LoRa, FSK, etc.).
 * @param packetType Packet type value.
 */
void sx126x_setPacketType(uint8_t packetType)
{
    sx126x_transfer(0x8A, &packetType, 1);
}

/**
 * @brief Get the current packet type.
 * @param packetType Pointer to store packet type.
 */
void sx126x_getPacketType(uint8_t* packetType)
{
    uint8_t buf[2];
    sx126x_transfer(0x11, buf, 2);
    *packetType = buf[1];
}

/**
 * @brief Set transmit parameters (power and ramp time).
 * @param power Transmit power.
 * @param rampTime Ramp time value.
 */
void sx126x_setTxParams(uint8_t power, uint8_t rampTime)
{
    uint8_t buf[2];
    buf[0] = power;
    buf[1] = rampTime;
    sx126x_transfer(0x8E, buf, 2);
}

/**
 * @brief Set LoRa modulation parameters.
 * @param sf Spreading factor.
 * @param bw Bandwidth.
 * @param cr Coding rate.
 * @param ldro Low data rate optimization.
 */
void sx126x_setModulationParamsLoRa(uint8_t sf, uint8_t bw, uint8_t cr, uint8_t ldro)
{
    uint8_t buf[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    buf[0] = sf;
    buf[1] = bw;
    buf[2] = cr;
    buf[3] = ldro;
    sx126x_transfer(0x8B, buf, 8);
}

/**
 * @brief Set FSK modulation parameters.
 * @param br Bit rate.
 * @param pulseShape Pulse shape.
 * @param bandwidth Bandwidth.
 * @param Fdev Frequency deviation.
 */
void sx126x_setModulationParamsFSK(uint32_t br, uint8_t pulseShape, uint8_t bandwidth, uint32_t Fdev)
{
    uint8_t buf[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    buf[0] = br >> 16;
    buf[1] = br >> 8;
    buf[2] = br;
    buf[3] = pulseShape;
    buf[4] = bandwidth;
    buf[5] = Fdev >> 16;
    buf[6] = Fdev >> 8;
    buf[7] = Fdev;
    sx126x_transfer(0x8B, buf, 8);
}

/**
 * @brief Set LoRa packet parameters.
 * @param preambleLength Preamble length.
 * @param headerType Header type.
 * @param payloadLength Payload length.
 * @param crcType CRC type.
 * @param invertIq Invert IQ.
 */
void sx126x_setPacketParamsLoRa(uint16_t preambleLength, uint8_t headerType, uint8_t payloadLength, uint8_t crcType, uint8_t invertIq)
{
    uint8_t buf[9] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    buf[0] = preambleLength >> 8;
    buf[1] = preambleLength;
    buf[2] = headerType;
    buf[3] = payloadLength;
    buf[4] = crcType;
    buf[5] = invertIq;
    sx126x_transfer(0x8C, buf, 9);
}

/**
 * @brief Set FSK packet parameters.
 * @param preambleLength Preamble length.
 * @param preambleDetector Preamble detector.
 * @param syncWordLength Sync word length.
 * @param addrComp Address comparison.
 * @param packetType Packet type.
 * @param payloadLength Payload length.
 * @param crcType CRC type.
 * @param whitening Whitening enable.
 */
void sx126x_setPacketParamsFSK(uint16_t preambleLength, uint8_t preambleDetector, uint8_t syncWordLength, uint8_t addrComp, uint8_t packetType, uint8_t payloadLength, uint8_t crcType, uint8_t whitening)
{
    uint8_t buf[9] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    buf[0] = preambleLength >> 8;
    buf[1] = preambleLength;
    buf[2] = preambleDetector;
    buf[3] = syncWordLength;
    buf[4] = addrComp;
    buf[5] = packetType;
    buf[6] = payloadLength;
    buf[7] = crcType;
    buf[8] = whitening;
    sx126x_transfer(0x8C, buf, 9);
}

/**
 * @brief Set CAD (Channel Activity Detection) parameters.
 * @param cadSymbolNum Number of CAD symbols.
 * @param cadDetPeak CAD detection peak.
 * @param cadDetMin CAD detection minimum.
 * @param cadExitMode CAD exit mode.
 * @param cadTimeout CAD timeout value.
 */
void sx126x_setCadParams(uint8_t cadSymbolNum, uint8_t cadDetPeak, uint8_t cadDetMin, uint8_t cadExitMode, uint32_t cadTimeout)
{
    uint8_t buf[7];
    buf[0] = cadSymbolNum;
    buf[1] = cadDetPeak;
    buf[2] = cadDetMin;
    buf[3] = cadExitMode;
    buf[4] = cadTimeout >> 16;
    buf[5] = cadTimeout >> 8;
    buf[6] = cadTimeout;
    sx126x_transfer(0x88, buf, 7);
}

/**
 * @brief Set the buffer base address for TX and RX.
 * @param txBaseAddress TX base address.
 * @param rxBaseAddress RX base address.
 */
void sx126x_setBufferBaseAddress(uint8_t txBaseAddress, uint8_t rxBaseAddress)
{
    uint8_t buf[2];
    buf[0] = txBaseAddress;
    buf[1] = rxBaseAddress;
    sx126x_transfer(0x8F, buf, 2);
}

/**
 * @brief Set the number of LoRa symbols for timeout.
 * @param symbnum Number of symbols.
 */
void sx126x_setLoRaSymbNumTimeout(uint8_t symbnum)
{
    sx126x_transfer(0xA0, &symbnum, 1);
}
        
/**
 * @brief Get the device status.
 * @param status Pointer to store status value.
 */
void sx126x_getStatus(uint8_t* status)
{
    uint8_t buf;
    sx126x_transfer(0xC0, &buf, 1);
    *status = buf;
}

/**
 * @brief Get the RX buffer status (payload length and buffer pointer).
 * @param payloadLengthRx Pointer to store payload length.
 * @param rxStartBufferPointer Pointer to store buffer pointer.
 */
void sx126x_getRxBufferStatus(uint8_t* payloadLengthRx, uint8_t* rxStartBufferPointer)
{
    uint8_t buf[3];
    sx126x_transfer(0x13, buf, 3);
    *payloadLengthRx = buf[1];
    *rxStartBufferPointer = buf[2];
}

/**
 * @brief Get the packet status (RSSI, SNR, signal RSSI).
 * @param rssiPkt Pointer to store packet RSSI.
 * @param snrPkt Pointer to store SNR.
 * @param signalRssiPkt Pointer to store signal RSSI.
 */
void sx126x_getPacketStatus(uint8_t* rssiPkt, uint8_t* snrPkt, uint8_t* signalRssiPkt)
{
    uint8_t buf[4];
    sx126x_transfer(0x14, buf, 4);
    *rssiPkt = buf[1];
    *snrPkt = buf[2];
    *signalRssiPkt = buf[3];
}

/**
 * @brief Get the instantaneous RSSI value.
 * @param rssiInst Pointer to store RSSI value.
 */
void sx126x_getRssiInst(uint8_t* rssiInst)
{
    uint8_t buf[2];
    sx126x_transfer(0x15, buf, 2);
    *rssiInst = buf[1];
}

/**
 * @brief Get statistics (packets received, CRC errors, header errors).
 * @param nbPktReceived Pointer to store number of packets received.
 * @param nbPktCrcError Pointer to store number of CRC errors.
 * @param nbPktHeaderErr Pointer to store number of header errors.
 */
void sx126x_getStats(uint16_t* nbPktReceived, uint16_t* nbPktCrcError, uint16_t* nbPktHeaderErr)
{
    uint8_t buf[7];
    sx126x_transfer(0x10, buf, 7);
    *nbPktReceived = (buf[1] << 8) | buf[2];
    *nbPktCrcError = (buf[3] << 8) | buf[4];
    *nbPktHeaderErr = (buf[5] << 8) | buf[6];
}

/**
 * @brief Reset statistics counters.
 */
void sx126x_resetStats()
{
    uint8_t buf[6] = {0, 0, 0, 0, 0, 0};
    sx126x_transfer(0x00, buf, 6);
}

/**
 * @brief Get device error status.
 * @param opError Pointer to store error code.
 */
void sx126x_getDeviceErrors(uint16_t* opError)
{
    uint8_t buf[3];
    sx126x_transfer(0x17, buf, 3);
    *opError = buf[2];
}

/**
 * @brief Clear device error status.
 */
void sx126x_clearDeviceErrors()
{
    uint8_t buf[2] = {0, 0};
    sx126x_transfer(0x07, buf, 2);
}

/**
 * @brief Fix LoRa bandwidth 500 kHz configuration.
 * @param bw Bandwidth value.
 */
void sx126x_fixLoRaBw500(uint32_t bw)
{
    uint8_t packetType;
    sx126x_getPacketType(&packetType);
    uint8_t value;
    sx126x_readRegister(SX126X_REG_TX_MODULATION, &value, 1);
    if ((packetType == SX126X_LORA_MODEM) && (bw == 500000)) value &= 0xFB;
    else value |= 0x04;
    sx126x_writeRegister(SX126X_REG_TX_MODULATION, &value, 1);
}

/**
 * @brief Fix antenna resistance configuration.
 */
void sx126x_fixResistanceAntenna()
{
    uint8_t value;
    sx126x_readRegister(SX126X_REG_TX_CLAMP_CONFIG, &value, 1);
    value |= 0x1E;
    sx126x_writeRegister(SX126X_REG_TX_CLAMP_CONFIG, &value, 1);
}

/**
 * @brief Fix RX timeout configuration.
 */
void sx126x_fixRxTimeout()
{
    uint8_t value = 0x00;
    sx126x_writeRegister(SX126X_REG_RTC_CONTROL, &value, 1);
    sx126x_readRegister(SX126X_REG_EVENT_MASK, &value, 1);
    value = value | 0x02;
    sx126x_writeRegister(SX126X_REG_EVENT_MASK, &value, 1);
}

/**
 * @brief Fix inverted IQ configuration.
 * @param invertIq Invert IQ value.
 */
void sx126x_fixInvertedIq(uint8_t invertIq)
{
    uint8_t value;
    sx126x_readRegister(SX126X_REG_IQ_POLARITY_SETUP, &value, 1);
    if (invertIq) value |= 0x04;
    else value &= 0xFB;
    sx126x_writeRegister(SX126X_REG_IQ_POLARITY_SETUP, &value, 1);
}

/**
 * @brief Transfer data to/from the SX126x device (simple version).
 * @param opCode Operation code.
 * @param data Pointer to data array.
 * @param nData Number of bytes to transfer.
 */
void sx126x_transfer(uint8_t opCode, uint8_t* data, uint8_t nData)
{
    sx126x_transfer(opCode, data, nData, NULL, 0, true);
}

/**
 * @brief Transfer data to/from the SX126x device (advanced version).
 * @param opCode Operation code.
 * @param data Pointer to data array.
 * @param nData Number of bytes to transfer.
 * @param address Pointer to address array.
 * @param nAddress Number of address bytes.
 * @param read True for read operation, false for write.
 */
void sx126x_transfer(uint8_t opCode, uint8_t* data, uint8_t nData, uint8_t* address, uint8_t nAddress, bool read)
{
    if (sx126x_busyCheck(SX126X_BUSY_TIMEOUT)) return;

    digitalWrite(sx126x_nss, LOW);
    sx126x_spi->beginTransaction(SPISettings(sx126x_spiFrequency, MSBFIRST, SPI_MODE0));
    sx126x_spi->transfer(opCode);
    for (int8_t i=0; i<nAddress; i++) sx126x_spi->transfer(address[i]);
    for (int8_t i=0; i<nData; i++) {
        if (read) data[i] = sx126x_spi->transfer(data[i]);
        else sx126x_spi->transfer(data[i]);
    }
    sx126x_spi->endTransaction();
    digitalWrite(sx126x_nss, HIGH);
}