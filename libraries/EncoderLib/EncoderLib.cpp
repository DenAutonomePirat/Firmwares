/*
  Morse.cpp - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/

#include "Arduino.h"
#include "EncoderLib.h"

Encoder::Encoder(int dataPin,int clockPin,int chipSelect)
{
  pinMode(clockPin, OUTPUT);
  pinMode(chipSelect, OUTPUT);
  _clockPin = clockPin;
  _chipSelect = chipSelect;
  _dataPin = dataPin;
}

int Encoder::getEncoder()
{
  digitalWrite(_chipSelect,LOW);
  byte _incoming = shiftIn(_dataPin, _clockPin, MSBFIRST);
  _sensorValue = _incoming<<2;
  _incoming = shiftIn(_dataPin, _clockPin, MSBFIRST);
  digitalWrite(_chipSelect,HIGH);
  _incoming = _incoming >> 6;
  _sensorValue |= _incoming;
  return _sensorValue;
}

int Encoder::getAbsolute()

{
  _pos = getEncoder();
  if (_lastPos > 0x0300 && _pos < 0x00FF)_metaPos++;
  if (_lastPos < 0x00FF && _pos > 0x0300)_metaPos--;
  _pos1 = _metaPos << 10;
  _pos1 = _pos1 | _pos;
  _lastPos = _pos;
  return _pos1;
}