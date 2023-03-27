#include <Arduino.h>
#include "LedShift.h"

#define BLINK_DELAY 150

LedShift::LedShift(uint8_t pin_DAI, uint8_t pin_DCK, uint8_t pin_LAT, uint8_t pins)
{
  _count = 0;
  _timer = millis() + BLINK_DELAY;
  _pin_DAI = pin_DAI;
  _pin_DCK = pin_DCK;
  _pin_LAT = pin_LAT;
  _pins = min(pins, 64);
  for (int pin = 0; pin < _pins; pin++)
  {
   _mode[pin] = ledOff;
  }
  pinMode(_pin_DAI, OUTPUT);
  pinMode(_pin_DCK, OUTPUT);
  pinMode(_pin_LAT, OUTPUT);
  digitalWrite(_pin_DAI, LOW);
  digitalWrite(_pin_DCK, LOW);
  digitalWrite(_pin_LAT, LOW);
  _send();
}

// send data
void LedShift::_send()
{
  // get bit masks
  uint8_t dataPort = digitalPinToPort(_pin_DAI);
  uint8_t dataMask = digitalPinToBitMask(_pin_DAI);
  uint8_t clockPort = digitalPinToPort(_pin_DCK);
  uint8_t clockMask = digitalPinToBitMask(_pin_DCK);
  uint8_t oldSREG = SREG;
  cli();
  uint8_t val = _count | 0x08;
  for (uint8_t pin = _pins; pin-- > 0;)
  {
    (_mode[pin] & val) > 0 ? *portOutputRegister(dataPort) |= dataMask : *portOutputRegister(dataPort) &= ~dataMask;
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

void LedShift::set(uint8_t pin, led_t mode)
{
  if (pin < _pins)
  {
    if (_mode[pin] != mode)
    {
      _mode[pin] = mode;
      _update = true;
    }
  }
}

void LedShift::setAll(led_t mode)
{
  for (int pin = 0; pin < _pins; pin++)
  {
    _mode[pin] = mode;
  }
  _update = true;
}

void LedShift::handle()
{
  if (millis() >= _timer)
  {
    _timer += BLINK_DELAY;
    _count = (_count + 1) & 0x07;
    _update = true;
  }
  if (_update)
  {
    _send();
    _update = false;
  }
}
