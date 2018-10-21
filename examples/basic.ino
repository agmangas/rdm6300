#include "rdm630.h"

const byte RX_PIN = 2;
const byte TX_PIN = 3;

RDM6300 rfid(RX_PIN, TX_PIN);

String tagId;

void setup() {
  Serial.begin(9600);
  rfid.begin();
  Serial.println(">> Starting program");
}

void loop() {
  tagId = rfid.getTagId();

  if (tagId.length()) {
    Serial.print(millis());
    Serial.print(" :: Tag ID :: ");
    Serial.println(tagId);
  }
}
