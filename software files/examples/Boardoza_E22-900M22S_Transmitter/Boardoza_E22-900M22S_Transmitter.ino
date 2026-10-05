#include <Arduino.h>
#include <SX126x.h>

SX126x LoRa;

char message[] = "Boardoza LoRa Test";
uint8_t nBytes = sizeof(message);
uint8_t counter = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Begin LoRa radio");

  int8_t nssPin   = 5;
  int8_t resetPin = 2;
  int8_t busyPin  = 4;
  int8_t irqPin   = 26;   
  int8_t txenPin  = -1;
  int8_t rxenPin  = -1;

  if (!LoRa.begin(nssPin, resetPin, busyPin, irqPin, txenPin, rxenPin)) {
    Serial.println("LoRa init failed!");
    while (1);
  }

  // TCXO ayarı (E22 SX1262'de gerekli)
  LoRa.setDio3TcxoCtrl(SX126X_DIO3_OUTPUT_1_8, SX126X_TCXO_DELAY_10);

  // Frekans → 868 MHz (TR/EU için)
  LoRa.setFrequency(868000000);

  // Güç
  LoRa.setTxPower(22, SX126X_TX_POWER_SX1262);

  // Modülasyon
  LoRa.setLoRaModulation(7, 125000, 5);

  // Paket
  LoRa.setLoRaPacket(SX126X_HEADER_EXPLICIT, 12, 13, true);

  // Sync word
  LoRa.setSyncWord(0x3444);

  Serial.println("LoRa init OK!");
}

void loop() {
  LoRa.beginPacket();
  LoRa.write(message, nBytes);
  LoRa.write(counter);
  LoRa.endPacket();
  LoRa.wait();

  Serial.print("[TX] ");
  Serial.print(message);
  Serial.print(" ");
  Serial.println(counter++);

  delay(5000);
}
