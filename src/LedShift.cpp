#include <Arduino.h>
#include "LedShift.h"

#define BLINK_DELAY 150

LedShift::LedShift(uint8_t pin_DAI, uint8_t pin_DCK, uint8_t pin_LAT)
{
  _pin_DAI = pin_DAI;
  _pin_DCK = pin_DCK;
  _pin_LAT = pin_LAT;
  _count = 0;
  _state = 0;
  _timer = millis() + BLINK_DELAY;
  for (int pin = 0; pin < 16; pin++)
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

// send 16 bit
void LedShift::_send()
{
  // get bit masks
  uint8_t dataPort = digitalPinToPort(_pin_DAI);
  uint8_t dataMask = digitalPinToBitMask(_pin_DAI);
  uint8_t clockPort = digitalPinToPort(_pin_DCK);
  uint8_t clockMask = digitalPinToBitMask(_pin_DCK);
  uint16_t val = _state;
  for (uint8_t i = 16; i > 0; --i)
  {
    (val & 0x8000) > 0 ? *portOutputRegister(dataPort) |= dataMask : *portOutputRegister(dataPort) &= ~dataMask;
    *portOutputRegister(clockPort) |= clockMask;
    *portOutputRegister(clockPort) &= ~clockMask;
    val <<= 1;
  }
  // latch LAT signal
  clockPort = digitalPinToPort(_pin_LAT);
  clockMask = digitalPinToBitMask(_pin_LAT);
  *portOutputRegister(clockPort) |= clockMask;
  *portOutputRegister(clockPort) &= ~clockMask;
}

void LedShift::_set(uint8_t pin)
{
  switch (_mode[pin])
  {
  case ledOn:
    bitSet(_state, pin);
    break;
  case ledFast:
    bitWrite(_state, pin, bitRead(_count, 0));
    break;
  case ledMedium:
    bitWrite(_state, pin, bitRead(_count, 1));
    break;  
  case ledSlow:
    bitWrite(_state, pin, bitRead(_count, 2));
    break;
  default:
    bitClear(_state, pin);
  }
}

void LedShift::set(uint8_t pin, led_t mode)
{
  _mode[pin] = mode;
  _set(pin);
  _update = true;
}

void LedShift::set_all(led_t mode)
{
  for (int pin = 0; pin < 16; pin++)
  {
    _mode[pin] = mode;
    _set(pin);
  }
  _update = true;
}

void LedShift::handle()
{
  if (millis() >= _timer)
  {
    _timer += BLINK_DELAY;
    _count = (_count + 1) % 8;
    for (int pin = 0; pin < 16; pin++)
    {
      _set(pin);
    }
    _update = true;
  }
  if (_update)
  {
    _send();
    _update = false;
  }
}
