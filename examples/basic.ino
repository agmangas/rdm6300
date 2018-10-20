#include "rdm630.h"

const byte RX_PIN = 2;
const byte TX_PIN = 3;
const byte DATA_SIZE = 6;

RDM6300 rfid(RX_PIN, TX_PIN);

byte data[DATA_SIZE];
byte length;

void setup() {
  Serial.begin(9600);
  rfid.begin();
  Serial.println(">> Starting program");
}

void loop() {
  Serial.println(">> Reading...");

  if (rfid.getData(data, length)) {
    Serial.print(">> Data: ");
    Serial.println(RDM6300::toHexString(data, sizeof(data)));
  }
}
