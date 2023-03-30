#include <Arduino.h>
#include "AnalogIn.h"

#define FULL_SCALE ((1 << AD_RES) - 1)
#define HALF_SCALE (1 << (AD_RES - 1))

AnalogIn::AnalogIn(uint8_t pin, Analog_t type)
{
  _pin = pin;
  _filterConst = 1.0;
  _scale = 1.0;
  pinMode(_pin, INPUT);
  if (type == bipolar)
  {
    _offset = HALF_SCALE;
    _scalePos = _scale / HALF_SCALE;
    _scaleNeg = _scale / HALF_SCALE;
  }
  else
  {
    _offset = 0;
    _scalePos = _scale / FULL_SCALE;
    _scaleNeg = 0.0;
  }
}

AnalogIn::AnalogIn(uint8_t pin, Analog_t type, float timeConst) : AnalogIn(pin, type)
{
  _filterConst = 1.0 / timeConst;
}

void AnalogIn::handle()
{
  int _raw = raw();
  _value = (_filterConst * _raw * (_raw >= 0 ? _scalePos : _scaleNeg)) + (1.0 - _filterConst) * _value;
}

int AnalogIn::raw()
{
  return analogRead(_pin) - _offset;
}

void AnalogIn::calibrate()
{
  long sum = 0;
  for (int i = 0; i < 64; i++)
  {
    sum += analogRead(_pin);
  }
  _offset = (int)(sum / 64);
  _scalePos = (_offset < FULL_SCALE) ? _scale / (float)(FULL_SCALE - _offset) : 1.0;
  _scaleNeg = (_offset > 0)? _scale / (float)(_offset) : 1.0;
}

void AnalogIn::setRange(int16_t offset, int16_t range)
{
  _offset = offset;
  if (range > offset)
  {
    _scalePos = _scale / (float)(range-offset);
    _scaleNeg = 0;
  }
  else if (range < offset)
  {
    _scalePos = 0;
    _scaleNeg = _scale / (float)(offset-range);
  }
  else
  {
    _scalePos = 0;
    _scaleNeg = 0;
  }
}

void AnalogIn::setScale(float scale)
{
  _scale = scale;
}