#include <Arduino.h>
#include <SX126x.h>

SX126x LoRa;

void setup() {
  Serial.begin(9600);
  Serial.println("Begin LoRa receiver (ESP32-S2)");

  // ESP32-S2 pin mapping
  int8_t nssPin   = 34;   // CS (FSPICS0)
  int8_t resetPin = 2;    // NRST
  int8_t busyPin  = 4;    // BUSY
  int8_t irqPin   = 9;    // DIO1 (opsiyonel, gerekmezse -1 yap)
  int8_t txenPin  = -1;
  int8_t rxenPin  = -1;

  if (!LoRa.begin(nssPin, resetPin, busyPin, irqPin, txenPin, rxenPin)) {
    Serial.println("LoRa init failed!");
    while (1);
  }

  // TCXO ayarı (E22'de gerekli)
  LoRa.setDio3TcxoCtrl(SX126X_DIO3_OUTPUT_1_8, SX126X_TCXO_DELAY_10);

  // Türkiye için frekans
  LoRa.setFrequency(868000000);

  // Modülasyon parametreleri
  LoRa.setLoRaModulation(7, 125000, 5);

  // Paket parametreleri (13 byte payload: "HeLoRa World!" + counter)
  LoRa.setLoRaPacket(SX126X_HEADER_EXPLICIT, 12, 13, true);

  // Sync word
  LoRa.setSyncWord(0x3444);

  Serial.println("-- LORA RECEIVER READY --");
}

void loop() {
  // Yeni paket bekle
  LoRa.request();
  LoRa.wait();

  // Mesaj oku
  const uint8_t msgLen = LoRa.available() - 1;
  char message[msgLen + 1];
  uint8_t counter;

  uint8_t i = 0;
  while (LoRa.available() > 1) {
    message[i++] = LoRa.read();
  }
  message[i] = '\0';
  counter = LoRa.read();

  // Gelen mesajı yazdır
  Serial.print("[RX] ");
  Serial.print(message);
  Serial.print("  Counter: ");
  Serial.println(counter);

  // Sinyal bilgileri
  Serial.print("RSSI = ");
  Serial.print(LoRa.packetRssi());
  Serial.print(" dBm | SNR = ");
  Serial.print(LoRa.snr());
  Serial.println(" dB");

  uint8_t status = LoRa.status();
  if (status == SX126X_STATUS_CRC_ERR) Serial.println("CRC error");
  else if (status == SX126X_STATUS_HEADER_ERR) Serial.println("Packet header error");

  Serial.println();
}
