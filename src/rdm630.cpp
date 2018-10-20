#include "rdm630.h"
#include <string.h>

rdm630::rdm630(byte yPinRx, byte yPinTx) : _rfid(yPinRx, yPinTx),
                                           _state(WAITING_FOR_STX)
{
}

void rdm630::begin()
{
    _rfid.begin(9600);
}

void rdm630::listen()
{
    _rfid.listen();
}

void rdm630::reset()
{
    memset(_data, 0, sizeof(_data));
    _state = WAITING_FOR_STX;
}

bool rdm630::getData(byte *data, byte &length)
{
    if (!_rfid.isListening())
    {
        listen();
    }

    unsigned long ini = millis();

    while (millis() < (ini + RDM630_READ_WAIT_MS))
    {
        while (_rfid.available() > 0)
        {
            _state = dataParser(_state, _rfid.read());

            if (_state == DATA_VALID)
            {
                break;
            }
        }

        if (_state == DATA_VALID)
        {
            break;
        }
    }

    if (_state != DATA_VALID)
    {
        return false;
    }

    length = sizeof(_data);
    memcpy(data, _data, sizeof(_data));

    reset();

    return true;
}

byte rdm630::asciiCharToNum(byte data)
{
    return (data > '9' ? data - '0' - 7 : data - '0');
}

rdm630::state rdm630::dataParser(state s, byte c)
{
    switch (s)
    {
    case WAITING_FOR_STX:
    case DATA_VALID:
        if (c == STX)
        {
            _iNibbleCtr = -1;
            return READING_DATA;
        }

        break;
    case READING_DATA:
        if (++_iNibbleCtr < 12)
        {
            _data[_iNibbleCtr >> 1] = ((_iNibbleCtr & 0x1) == 0 ? asciiCharToNum(c) << 4 : _data[_iNibbleCtr >> 1] + asciiCharToNum(c));
            return READING_DATA;
        }

        if (c != ETX)
        {
            // Expected end character, but got something else
            return WAITING_FOR_STX;
        }

        for (int i = 0; i < 5; i++)
        {
            _data[5] ^= _data[i];
        }

        if (_data[5] != 0)
        {
            // Checksum doesn't match
            return WAITING_FOR_STX;
        }

        return DATA_VALID;
    default:
        return WAITING_FOR_STX;
    }

    return WAITING_FOR_STX;
}
