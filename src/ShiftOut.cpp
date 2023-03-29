#include <Arduino.h>
#include "ShiftOut.h"

ShiftOut::ShiftOut(uint8_t pin_DAI, uint8_t pin_DCK, uint8_t pin_LAT, uint8_t pins)
{
  _pin_DAI = pin_DAI;
  _pin_DCK = pin_DCK;
  _pin_LAT = pin_LAT;
  _pins = min(pins, 64);
  pinMode(_pin_DAI, OUTPUT);
  pinMode(_pin_DCK, OUTPUT);
  pinMode(_pin_LAT, OUTPUT);
  digitalWrite(_pin_DAI, LOW);
  digitalWrite(_pin_DCK, LOW);
  digitalWrite(_pin_LAT, LOW);
  _send();
}

// send data
void ShiftOut::_send()
{
  // get bit masks
  uint8_t dataPort = digitalPinToPort(_pin_DAI);
  uint8_t dataMask = digitalPinToBitMask(_pin_DAI);
  uint8_t clockPort = digitalPinToPort(_pin_DCK);
  uint8_t clockMask = digitalPinToBitMask(_pin_DCK);
  uint8_t oldSREG = SREG;
  noInterrupts();
  for (uint8_t pin = _pins; pin-- > 0;)
  {
    bitRead(_state[pin >> 3], pin & 0x07) ? *portOutputRegister(dataPort) |= dataMask : *portOutputRegister(dataPort) &= ~dataMask;
    *portOutputRegister(clockPort) |= clockMask;
    *portOutputRegister(clockPort) &= ~clockMask;
  }
  // latch LAT signal
  clockPort = digitalPinToPort(_pin_LAT);
  clockMask = digitalPinToBitMask(_pin_LAT);
  *portOutputRegister(clockPort) |= clockMask;
  *portOutputRegister(clockPort) &= ~clockMask;
  SREG = oldSREG;
}

void ShiftOut::setPin(uint8_t pin, bool state)
{
  if (pin < _pins)
  {
    if (state != bitRead(_state[pin >> 3], pin & 0x07))
    {
      bitWrite(_state[pin >> 3], pin & 0x07, state);
      _update = true;
    }
  }
}

void ShiftOut::setAll(bool state)
{
  for (int pin = 0; pin < _pins; pin++)
  {
    bitWrite(_state[pin >> 3], pin & 0x07, state);
  }
  _update = true;
}

void ShiftOut::handle()
{
  if (_update)
  {
    _send();
    _update = false;
  }
}
