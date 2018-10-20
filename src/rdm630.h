#ifndef RDM630_H
#define RDM630_H

#include "Arduino.h"
#include "SoftwareSerial.h"

const unsigned long RDM630_READ_WAIT_MS = 2000;

class rdm630
{
public:
  rdm630(byte yPinRx, byte yPinTx);
  void begin();
  void listen();
  bool getData(byte *data, byte &length);

private:
  typedef enum
  {
    WAITING_FOR_STX,
    READING_DATA,
    DATA_VALID
  } state;

  static const int STX = 2;
  static const int ETX = 3;

  byte asciiCharToNum(byte data);
  state dataParser(state s, byte c);
  void reset();

  SoftwareSerial _rfid;
  state _state;
  int _iNibbleCtr;
  byte _data[6];
};

#endif // RDM630_H
